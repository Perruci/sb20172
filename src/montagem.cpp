#include "../include/montagem.h"

Montagem::Montagem(std::string inputFile, std::string outputFile, std::vector<Mnemonic>& instructionList)
{
    this->inputFileName =  inputFile;
    this->outputFileName = outputFile;
    this->setInputExtension(".mcr");
    this->setOutputExtension (".o");
    this->fileText.open(inputFileName);
    this->fileOutput.open(outputFileName);

    /* Pass as reference the instructionList */
    this->instructionList = instructionList;
}

Montagem::~Montagem()
{
    // Sem preocupações com o close
    fileText.close();
    fileOutput.close();
}

std::string Montagem::setOutputExtension(std::string extension)
{
    this->outputFileName = string_ops::setOutputExtension(inputFileName, extension);

    return outputFileName;
}

std::string Montagem::setInputExtension(std::string extension)
{
    this->inputFileName = string_ops::setOutputExtension(inputFileName, extension);

    return inputFileName;
}

//Metodo principal que vai realizar a montagem
bool Montagem::run(){
    bool check_section_text = false;  //passa a ser true quando acharmos a secao text
    bool now_section_text = false;    //indica que atualmente o programa esta na secao de texto
    bool now_section_data = false;    //indica que atualmente o programa esta na secao de data
    int contador_tokens = 0;          //sera utilizado para navegar pela tokensList
    int contador_endereco = 0;        //sera utilizado para determinar o endereco de cada token do programa
    int haveRotuloInLine = 0;            //controle para caso aparecam dois rotulos na mesma linha
    int contador_de_linhas = 0;       //Controle para saber em qual linha do programa esta algum erro


    //Fecha e abre o arquivo do codigo para atualizar o ponteiro de arquivo, caso alguma outra funcao tenha usado ele
    this->reopenCodeFile();

    std::string line;
    std::string word;

    //Loop para fazer a montagem
    // para cada linha..
    while (getline(this->fileText, line))
    {
        // transforma para minuscula
        line = string_ops::minuscula(line);
        contador_de_linhas++;               //incrementa a linha

        std::stringstream lineStream (line);
        haveRotuloInLine = 0;               //prepara a variavel para  analisar o proximo rotulo

        //antes de analisar cada linha, inicializa novamente as variaveis de controle do erro lexico
        this->lineIsInstruction = false;     
        this->numberOfOperandsInLine = 0;     
        this->lineIsSpace = false;           
        this->numberOfArgumentsInSpace = 0;   
        this->lineIsConst = false;          
        this->numberOfArgumentsInConst = 0;   

        //Loop para analisar cada termo um a um
        while (lineStream >> word)
        {
            //coloca o termo na lista
            this->tokensList.push_back(Token(word));

            //Caso apareca a secao de texto ou de data, ja trata ela
            if(word == "section"){
                contador_tokens++;
                this->trata_section(check_section_text, now_section_data, now_section_text, lineStream, word, contador_de_linhas);
                //vai para a proxima linha, nao ha necessidade de analisar mais nada nessa
                break;
            }
            
            //se o token for um rotulo, trata ele
            if (this->tokensList[contador_tokens].isRotulo(word, line, instructionList)){
                this ->trataRotulo_altoNivel(contador_tokens, word, contador_endereco, haveRotuloInLine, contador_de_linhas);
            }

            //Testa se o token atual eh uma diretiva, caso seja prepara as variaveis para analisar o resto da linha
            if(this->isDiretiva(this->tokensList[contador_tokens])){
                this->trata_diretivas(contador_de_linhas, now_section_data, contador_tokens);
                //adiciona 1 no contador de enderecos, caso seja space com mais de um "campo" eh importante incrementar Y-1 enderecos depois
                contador_endereco++;
            }

            //Testa se o token atual eh uma instrucao, caso seja prepara as variaveis para receber os argumentos
            if (this-> isInstruction(this->tokensList[contador_tokens])){
                this->trata_instructions(contador_de_linhas, now_section_text, contador_tokens);
                contador_endereco++;
            }
            contador_tokens++;
        }
        //No fim de cada linha, verifica se houve algum erro lexico por falta ou excesso de argumentos
        this-> checkLexicalError(contador_de_linhas);
    }
    //Se nao encontrarmos a secao de text no arquivo, temos um erro
    if (!check_section_text){
        std::cout << "Erro semantico, nao ha secao text no arquivo\n";
    }
    this->printRotulos();
    this->printOutput();
    return true;
}

void Montagem::trata_rotulos (std::string token, int tipo_rotulo, int &endereco){
    //Retira, caso tenha, o :
    token = string_ops::trunca_nome(token, ':');

    //se for a declaracao de um rotulo, chama a rotina pra tratar isso
    if(tipo_rotulo == tipo_rotulo::declaracao)
    {
        this->declaracao_de_rotulo(token, endereco);
        return;
    }

    //se for chamada de um rotulo, chama a rotina pra tratar isso
    if (tipo_rotulo == tipo_rotulo::chamada)
    {
        this->chamada_de_rotulo(token, endereco);
        return;
    }

    return;
}

void Montagem::declaracao_de_rotulo(std::string token, int &endereco)
{
    for (size_t i = 0; i < this->rotulosList.size(); i++)
    {
        //se ele ja existir na lista, devemos apenas atualizar o endereco de declaracao dele e o estado
        if (token == this->rotulosList[i].name)
        {
            this->rotulosList[i].setState(endereco);
            
            //Vai na saida e atualiza todos os lugares onde o rotulo foi chamado, colocando o endereco real dele
            //Ainda nao vai tratar as consts e o sppaces com mais de 1 space
            for (size_t j = 0; j < this->rotulosList[i].addressList.size(); j++){
                //se o rotulo for uma const, atualiza com o valor da constante
                if (this->rotulosList[i].isConst){
                    this->outputFileList[this->rotulosList[i].addressList[j]] = this->rotulosList[i].constValue;
                }

                //se o rotulo for um space, atualiza com o endereco mais o valor que ja se encontra la
                else if (this->rotulosList[i].isSpace){
                    //So atualiza se o endereco a mais que a pessoa quer faz parte do espaco guardado para o space
                    if (this->outputFileList[this->rotulosList[i].addressList[j]] < this->rotulosList[i].spaceQuantity){
                        this->outputFileList[this->rotulosList[i].addressList[j]] += this->rotulosList[i].address;
                    } else {
                        std::cout << "Erro semantico, espaco da diretiva space foi estourado\n";
                        this->outputFileList[this->rotulosList[i].addressList[j]] += this->rotulosList[i].address;
                    }
                }
                //No caso normal, so coloca o endereco de declaracao la
                else {
                    this->outputFileList[this->rotulosList[i].addressList[j]] = this->rotulosList[i].address;
                }
                
            }
            
            return;
        }
    }
    //se ele ainda nao existir na lista, criamos ele agora
    Rotulo rotulo (token, true, endereco);
    rotulosList.push_back(rotulo);
    return;
}

void Montagem::chamada_de_rotulo(std::string token, int &endereco)
{
    for (size_t i = 0; i < this->rotulosList.size(); i++)
    {
        //se ele ja existir na lista, devemos colocar no lugar o endereco dele caso ja esteja declarado, ou adicionar
        //esse endereco na lista de lugares onde ele eh chamado para tratar depois
        if (token == this->rotulosList[i].name)
        {
            if(this->rotulosList[i].alreadyDeclared)
            {
                //se for uma constante, adiciona o valor dela no arquivo final e incrementa o contador de enderecos
                if(this->rotulosList[i].isConst){
                    this->outputFileList.push_back(rotulosList[i].constValue);
                    endereco++;
                    return;
                }

                //se for um space, coloca o endereco de declaracao do rotulo (NAO FUNCIONARA PARA SPACES DIFERENTES DE 1, TRATAR DEPOIS)
                if(this->rotulosList[i].isSpace){
                    this->outputFileList.push_back(rotulosList[i].address);
                    endereco++;
                    return;
                }

                //caso contrario, eh um rotulo qualquer, adiciona o endereco de declaracao dele
                this->outputFileList.push_back(rotulosList[i].address);
                endereco++;
                return;
            } else
            {
                //Coloca na lista desse rotulo aquele endereco para ele ser arrumado posteriormente e incrementa o contador de enderecos e coloca 0 na lista de saida
                this->rotulosList[i].addressList.push_back(endereco);
                this->outputFileList.push_back(0);
                endereco++;
                return;
            }
        }
    }
    //Caso ainda nao exista um rotulo com esse nome
    // Cria um novo rótulo e adiciona-o a lista
    Rotulo rotulo (token, false, 0);
    rotulo.addressList.push_back(endereco);
    this->rotulosList.push_back(rotulo);
    this->outputFileList.push_back(0);
    endereco++;
    return;
}

void Montagem::printRotulos(){
    for (size_t i = 0; i < this->rotulosList.size(); i++){
        std::cout << this->rotulosList[i].name << ' ';
        for(size_t j = 0; j < this->rotulosList[i].addressList.size(); j++){
            std::cout << this->rotulosList[i].addressList[j] << ' ';
        }
        std::cout << std::endl;
    }
}

//Analisa cada um dos tokens lexicamente, para checar se ele apresenta erro lexico
//dependendo da posicao do token, regras diferentes devem ser analisadas para saber se o token eh valido,portanto seguem os modos de operacao
//const = espera um numero
//space = espera um numero
//argumento(op = a) = espera um rotulo
//rotulo = espera
bool Montagem::scannerLexico (std::string word, char operation){
    
    switch (operation){
        case 'a':
            //vai apresentar erro lexico se tiver algum caracter especial no token ou ele se iniciar com numero
            if ((word[0] == '0') || (word[0] == '1') || (word[0] == '2') || (word[0] == '3') || (word[0] == '4')
            || (word[0] == '5') || (word[0] == '6') || (word[0] == '7') || (word[0] == '8') || (word[0] == '9'))
            {
                return false;
            }
            return true;
    }

    //default
    return false;
    
}

//Confere se um certo token eh uma instrucao
bool Montagem::isInstruction(Token token){
    //percorre a lista de instrucao e testa para ver se o token eh instrucao
    for(size_t i = 0; i < this->instructionList.size(); i++){
        if (token.nome == this->instructionList[i].nome){
            //std::cout << "DEBBUG - instrucao " << token.nome << std::endl;
            return true;
        }
    }
    return false;
}

//Percorre a lista de intrucoes e retorna a quantidade de operandos que certa instrucao deseja receber
int Montagem::InstructionOperand(Token token){
    for (size_t i = 0; i < this->instructionList.size(); i++){
        if (token.nome == this->instructionList[i].nome){
            return this->instructionList[i].noperands;
        }
    }
    //se der erro retorna -1
    return -1;
}

//retorna o opcode da instrucao que esta sedo trabalhada
int Montagem::instructionOpcode(Token token){
    for (size_t i = 0; i < this->instructionList.size(); i++){
        if (token.nome == this-> instructionList[i].nome){
            return this->instructionList[i].opcode;
        }
    }
    //se der erro retorna -1
    return -1;
}

//Analisa as variaveis que tratam dos erros lexicos e checa se ha alguma inconsistencia
void Montagem::checkLexicalError(int line){
    //Verifica se a qtd de argumentos passadas para uma instrucao foi adequada
    if (this->lineIsInstruction){
        //se a quantidade de operandos estiver em 0 eh pq deu certo
        if(this->numberOfOperandsInLine == 0){
            return;
        }
        //se a quantidade de operandos estiver positiva eh pq faltam operandos na linha
        if(this->numberOfOperandsInLine > 0){
            std::cout<< "Erro lexico na linha " << line << ", faltaram " << this->numberOfOperandsInLine << " operandos.\n";
            return;
        }
        ////se a quantidade de operandos estiver negativa eh pq foram passados operandos a mais na linha
        if (this->numberOfOperandsInLine < 0){
            std::cout<< "Erro lexico na linha " << line << ", foram passados mais operandos que o esperado para a instrucao.\n";
            return;
        }
    }
    //Verifica se a qtd de argumentos passados para um space foi adequado
    if (this->lineIsSpace){
        //se a quantidade de operandos for 0 ou 1, tudo certo
        if(this->numberOfArgumentsInSpace >= 0){
            return;
        }
        if (this->numberOfArgumentsInSpace < 0){
            std::cout<< "Erro lexico na linha " << line << ", foram passados mais operandos que o esperado para a diretiva space.\n";
        }

    }
    //verifica se a qtd de argumentos passados para uma const foi adequado
    if (this->lineIsConst){
        //Se a qtd de operandos for 0, tudo certo
        if(this->numberOfArgumentsInConst == 0){
            return;
        }
        //Se a qtd de operadnos dor maior que 0, eh pq nao foi passado nenhum operando
        if (this->numberOfArgumentsInConst > 0){
            std::cout<< "Erro lexico na linha "<< line << ", nao foi passado nenhum valor para a diretiva const.\n";
        }
        //Se a qtd de operandos for menor que 0, eh pq passaram operandos ate demais
        if (this-> numberOfArgumentsInConst < 0){
            std::cout<< "Erro lexico na linha "<< line << ", foram passados mais operandos que o esperado para a diretiva const.\n";
        }
    }
}

//Faz toda a rotina para quando o token eh uma instrucao
void Montagem::trata_instructions (int line, bool now_section_text, int contador_tokens){
    //Se nao estivermos na secao de texto e uma instrucao aparecer, isso indica um erro
    //P.S: desculpa pela logica invertida no if
    if (!now_section_text){
        std::cout << "Erro semantico na linha " << line << " instrucao fora da secao de texto\n";
    }

    this->lineIsInstruction = true;
    //Pega a quantidade de operandos que essa instrucao requere
    this->numberOfOperandsInLine = this-> InstructionOperand(this->tokensList[contador_tokens]);
                    
    //Coloca o opcode da instrucao na lista para impressao e incrementa o contador de endereco
    this->outputFileList.push_back(this->instructionOpcode(this->tokensList[contador_tokens]));
}

//Confere se um token eh uma diretiva
bool Montagem::isDiretiva(Token token){
    //caso seja uma const, ja prepara as variaveis para ele
    if ((token.nome == "const") || (token.nome == "space")){
        return true;
    }
    return false;
}

//Faz toda a rotina de tratar as diretivas
void Montagem::trata_diretivas(int line, bool now_section_data, int contador_tokens){
    //Caso esteja fora da secao de data, isso representa um erro
    if(!now_section_data){
        std::cout << "Erro semantico na linha " << line << " diretiva fora da secao de data\n";
    }
    
    //Se for uma const, prepara as variaveis para const
    if (this->tokensList[contador_tokens].nome == "const"){
        this->lineIsConst = true;
        this->numberOfArgumentsInConst = 1;
        return;
    }
     //Se for uma const, prepara as variaveis para space
     if (this->tokensList[contador_tokens].nome == "space"){
        this->lineIsSpace = true;
        this->numberOfArgumentsInSpace = 1;
        return;
    }
}

//Faz toda a rotina para tratar as secoes section
void Montagem::trata_section(bool &check_section_text, bool &now_section_data, bool &now_section_text, std::stringstream &lineStream, std::string &word, int &contador_de_linhas){
    //pega o proximo token e analisa se ele eh data, text ou algum erro
    lineStream >> word;
    
    // esta na secao texto, atualiza variaveis 
    if (word == "text"){
        check_section_text = true;
        now_section_text = true;
        now_section_data = false;
    }
    // esta na secao data, atualiza variaveis
    else if (word == "data"){
        now_section_data = true;
        now_section_text = false;
    }
    else {
        std::cout << "Erro semantico na linha " << contador_de_linhas << " nao existe esse tipo de section\n";
    }
    
    //testa se ha algum token indesejado na linha do section
    if (lineStream >> word){
        std::cout << "Erro sintatico na linha " << contador_de_linhas << ", ha muitos termos na linha de section\n";
    }
}

//Uma das funcoes para tratar o rotulo, a mais "alto nivel" delas
void Montagem::trataRotulo_altoNivel(int contador_tokens, std::string word, int &contador_endereco, int &haveRotuloInLine, int contador_de_linhas){
    int tipo = tokensList[contador_tokens].KindOfRotulo(word);

    //testa se eh uma declaracao de rotulo e se ja tem outra declaracao na mesma linha
    if (tipo == tipo_rotulo::declaracao){
        //se ja tiver uma declaracao na linha sera erro
        if (haveRotuloInLine == 1){
            std::cout << "Erro lexico, declaracao de mais de um Rotulo na linha " << contador_de_linhas << "\n";
            } else
            {
                haveRotuloInLine = 1;
                this->trata_rotulos(word, tipo, contador_endereco);
            }
        }
        if (tipo == tipo_rotulo::chamada){
                    this->trata_rotulos(word, tipo, contador_endereco);
        }
}

//Imprime a lista de saida para o arquivo final
void Montagem::printOutput(){
    for(size_t i = 0; i < this->outputFileList.size(); i++){
        this->fileOutput << this->outputFileList[i] << " ";
        std::cout << this->outputFileList[i] << " ";
    }
    std::cout<< std::endl;
}
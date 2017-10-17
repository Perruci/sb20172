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

    /***********************************************************************************************************************************************************/
    //Serie de variaveis booleanas para indicar do que a linha atual do programa se trata e facilitar a decisao do que deve ser feito
    bool lineIsInstruction = false;     //caso ativo, significa que esperamos argumentos nos proximos tokens
    int numberOfOperandsInLine = 0;     //vai ser um contador decrescente para checar se a quantidade de operandos requeridos por uma instrucao foi conferida
                                        //Da suporte a variavel lineIsInstruction

    bool lineIsSpace = false;           //caso ativo, significa que podemos nao ter proximo token ou ter um numero
    int numberOfArgumentsInSpace = 0;   //vai ser um contador decrescente para checar se mais de um argumento foi passado para o space
                                        //Da suporte a variavel lineIsSpace

    bool lineIsConst = false;           //caso ativo, significa que esperamos um numero no proximo token
    int numberOfArgumentsInConst = 0;   //vai ser um contador decrescente para checar se mais de um argumento foi passado para o const
                                        //Da suporte a variavel lineIsConst
    /***********************************************************************************************************************************************************/

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

        //Loop para analisar cada termo um a um
        while (lineStream >> word)
        {
            //coloca o termo na lista
            this->tokensList.push_back(Token(word));

            if(word == "section"){
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
                if (!(lineStream >> word)){
                    std::cout << "Erro sintatico na linha " << contador_de_linhas << ", ha muitos termos na linha de section\n";
                }
                //vai para a proxima linha, nao ha necessidade de analisar mais nada nessa
                break;
            }
            
            //se o token for um rotulo, trata ele
            if (tokensList[contador_tokens].isRotulo(word, line, instructionList)){
                int tipo = tokensList[contador_tokens].KindOfRotulo(word);

                //testa se eh uma declaracao de rotulo e se ja tem outra declaracao na mesma linha
                if (tipo == tipo_rotulo::declaracao)
                {
                    //se ja tiver uma declaracao na linha sera erro
                    if (haveRotuloInLine == 1)
                    {
                        std::cout << "Mais de um Rotulo na linha " << contador_de_linhas << "\n";
                        return false;
                    } else
                    {
                        haveRotuloInLine = 1;
                        this->trata_rotulos(word, tipo, contador_endereco);
                    }
                }
                if (tipo == tipo_rotulo::chamada)
                {
                    this->trata_rotulos(word, tipo, contador_endereco);
                }
            }

            //Testa se o token atual eh uma instrucao, caso seja prepara as variaveis para receber os argumentos
            if (this-> isInstruction(tokensList[contador_tokens])){
                
                //Se nao estivermos na secao de texto e uma instrucao aparecer, isso indica um erro
                //P.S: desculpa pela logica invertida no if
                if (!now_section_text){
                    std::cout << "Erro semantico na linha " << contador_de_linhas << " instrucao fora da secao de texto\n"
                }

                //Pega a quantidade de operandos que essa instrucao requere
                numberOfOperandsInLine = this-> InstructionOperand(tokensList[contador_tokens]);
                
                //Coloca o opcode da instrucao na lista para impressao e incrementa o contador de endereco
                this->outputFileList.push_back(this->instructionOpcode(tokensList[contador_tokens]));
                contador_endereco++;
            }
            contador_tokens++;
        }
    }
    //Se nao encontrarmos a secao de text no arquivo, temos um erro
    if (!check_section_text){
        std::cout << "Erro semantico, nao ha secao text no arquivo\n";
    }
    this->printRotulos();
    return true;
}

void Montagem::trata_rotulos (std::string token, int tipo_rotulo, int endereco){
    //Retira, caso tenha, o :
    token = string_ops::trunca_nome(token, ':');

    //se for a declaracao de um rotulo, chama a rotina pra tratar isso
    if(tipo_rotulo == tipo_rotulo::declaracao)
    {
        declaracao_de_rotulo(token, endereco);
        return;
    }

    //se for chamada de um rotulo, chama a rotina pra tratar isso
    if (tipo_rotulo == tipo_rotulo::chamada)
    {
        chamada_de_rotulo(token, endereco);
        return;
    }

    return;
}

void Montagem::declaracao_de_rotulo(std::string token, int endereco)
{
    for (size_t i = 0; i < rotulosList.size(); i++)
    {
        //se ele ja existir na lista, devemos apenas atualizar o endereco de declaracao dele e o estado
        if (token == rotulosList[i].name)
        {
            rotulosList[i].setState(endereco);
            //ADD UMA ROTINA PARA TRATAR ONDE O ROTULO JA FOI ENCONTRADO ANTES, OU SEJA,
            //VOLTAR NO ARQUIVO E ATUALIZAR OS ENDERECOS. PRECISO NA MONTAGEM
            return;
        }
    }
    //se ele ainda nao existir na lista, criamos ele agora
    Rotulo rotulo (token, true, endereco);
    rotulosList.push_back(rotulo);
    return;
}

void Montagem::chamada_de_rotulo(std::string token, int endereco)
{
    for (size_t i = 0; i < rotulosList.size(); i++)
    {
        //se ele ja existir na lista, devemos colocar no lugar o endereco dele caso ja esteja declarado, ou adicionar
        //esse endereco na lista de lugares onde ele eh chamado para tratar depois
        if (token == rotulosList[i].name)
        {
            if(rotulosList[i].alreadyDeclared)
            {
                //ADD UMA ROTINA PARA TRATAR A PARTE DA MONTAGEM
                return;
            } else
            {
                rotulosList[i].addressList.push_back(endereco);
                return;
            }
        }
    }
    //Caso ainda nao exista um rotulo com esse nome
    // Cria um novo rótulo e adiciona-o a lista
    Rotulo rotulo (token, false, 0);
    rotulo.addressList.push_back(endereco);
    rotulosList.push_back(rotulo);
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
            return true;
        }
    }
    return false;
}

//Percorre a lista de intrucoes e retorna a quantidade de operandos que certa instrucao deseja receber
int Montagem::InstructionOperand(Token token){
    for (size_t i; i < this->instructionList.size(); i++){
        if (token.nome == this->instructionList[i].nome){
            return this->instructionList[i].noperands;
        }
    }
    //se der erro retorna -1
    return -1;
}

//retorna o opcode da instrucao que esta sedo trabalhada
int Montagem::instructionOpcode(Token token){
    for (size_t i; i < this->instructionList.size(); i++){
        if (token.nome == this-> instructionList[i].nome){
            return this->instructionList[i].opcode;
        }
    }
    //se der erro retorna -1
    return -1;
}
#include "../include/pre_processamento.h"

Pre_Processamento::Pre_Processamento(std::string inputFile, std::string outputFile, std::vector<Mnemonic>& instructionList)
{
    this->inputFileName =  inputFile;
    this->outputFileName = outputFile;
    this->setOutputExtension(".pre");
    this->fileText.open(inputFileName);
    this->fileOutput.open(outputFileName);
    /* Pass as reference the instructionList */
    this->instructionList = instructionList;
}

Pre_Processamento::~Pre_Processamento()
{
    // Sem preocupações com o close
    fileText.close();
    fileOutput.close();
}

std::string Pre_Processamento::setOutputExtension(std::string extension)
{
    this->outputFileName = string_ops::setOutputExtension(outputFileName, extension);

    return outputFileName;
}

//Faz a etapa de pre-processamento e trata os EQU e IF, funcao criada sem utilizar a tokenizacao, pois estou com receio de que
//se fizermos a tokenizacao nessa parte estariamos violando a regra de passagem unica
bool Pre_Processamento::run(){
    bool check_section_text = false;  //passa a ser true quando acharmos a secao text
    int contador_tokens = 0;          //sera utilizado para navegar pela tokensList
    int contador_endereco = 0;        //sera utilizado para determinar o endereco de cada token do programa
    int haveRotuloInLine = 0;            //controle para caso aparecam dois rotulos na mesma linha
    int contador_de_linhas = 0;       //Controle para saber em qual linha do programa esta algum erro

    //inicializa as condicoes de IF como falsas
    this->lastIF = false;

    //Fecha e abre o arquivo do codigo para atualizar o ponteiro de arquivo, caso alguma outra funcao tenha usado ele
    this->reopenCodeFile();

    std::string line;
    std::string word;

    //Loop para fazer o pre-processamento
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
            // se encontra um comentário, pula a linha
            if (word[0] == ';')
                break;

            //coloca o termo na lista
            this->tokensList.push_back(Token(word));

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
            contador_tokens++;

            if (word == "text")
            {
                check_section_text = true;
            }

            if ((word == "equ"))
            {
                if(haveRotuloInLine < 1)
                {
                        std::cout << "EQU não refere-se a nenhum rótulo. Linha: " << contador_de_linhas << '\n';
                        return false;
                }
                // Trata EQU
                if(!trata_equ(line, contador_de_linhas))
                {
                    return false;
                }
            }
            if(word == "if")
            {
                //pega o rotulo que vem depois do if
                if(!(lineStream >> word))
                {
                    std::cout << "linha " << contador_de_linhas
                              << " IF não se refere a nenhum rótulo" << '\n';
                    return false;
                }
                bool sintaxeCorrect, statementResult;
                std::tie(sintaxeCorrect, statementResult) = trata_if(word, contador_de_linhas);
                if(!sintaxeCorrect)
                {
                    return false;
                }
                // se o resultado do IF for negativo
                if(!statementResult)
                {
                    // Pula uma linha
                    //getline(this->fileText, line); // != melhor solucao
                }
            }
        }
        this-> printLine(line);
    }
    //Ta aqui so pra debugar
    this->printRotulos();
    //this->writeTokensToOutput();
    return true;
}

/* Tratamento de Rótulos */

//checa a lista de rotulos procurando se determinado rotulo ja foi visto antes, retorno com o seguinte significado
// 0-> primeira vez que encontramos o rotulo
// 1xx-> rotulo ja encontrado e ja declarado (xx eh o indice da lista de rotulos onde esse rotulo se encontra)
// 2xx-> rotulo ja encontrado, mas ainda nao declarado (xx vide de cima)
int Pre_Processamento::RotuloAlreadyFound(std::string token){
    //percorre a lista de rotulos procurando pelo rotulo
    for (size_t i = 0; i < rotulosList.size() ; i++){
        //se encontrar o rotulo
        if (rotulosList[i].name == token){
            if (rotulosList[i].alreadyDeclared){
                return (100 + i);
            } else {
                return (200 + i);
            }
        }
    }
    return 0;
}

void Pre_Processamento::trata_rotulos (std::string token, int tipo_rotulo, int endereco){
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

void Pre_Processamento::declaracao_de_rotulo(std::string token, int endereco)
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

void Pre_Processamento::chamada_de_rotulo(std::string token, int endereco)
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

void Pre_Processamento::printRotulos(){
    for (size_t i = 0; i < rotulosList.size(); i++){
        std::cout << rotulosList[i].name << ' ';
        for(size_t j = 0; j < rotulosList[i].addressList.size(); j++){
            std::cout << rotulosList[i].addressList[j] << ' ';
        }
        std::cout << std::endl;
    }
}

/* Tratamento de EQU */
bool Pre_Processamento::trata_equ(std::string line, size_t contador_de_linhas)
{
    if(!rotulosList.back().setEQU(line))
    {
        std::cout << "Error on line: " << contador_de_linhas << " while assigning EQU value" << '\n';
        return false;
    }
    return true;
}

/* Tratamento de IF */
std::tuple<bool,bool> Pre_Processamento::trata_if(std::string word, size_t contador_de_linhas)
{
    // variaveis de retorno
    bool sintaxeCorrect = false;
    bool statementResult;

    int value;
    // Vertifica se word já é um rótulo declarado po EQU
    for (size_t i = 0; i < rotulosList.size() && !sintaxeCorrect; i++)
        if(word == rotulosList[i].name)
        {
            if(rotulosList[i].isEqu)
            {
                value = rotulosList[i].EquValue;
                // Encontrado rotulo! IF tem sintaxe correta
                sintaxeCorrect = true;
            } else
            {
                std::cout << "linha "<< contador_de_linhas
                          << ": Rótulo encontrado não foi declarado como EQU" << '\n';
                return std::make_tuple(false, false);
            }
        }
    if(!sintaxeCorrect)
    {
        std::cout << "linha: "<< contador_de_linhas
                  << ": rótulo referente ao IF não encontrado"<< '\n';
      return std::make_tuple(false, false);
    }

    if(value == 0)
        statementResult = false;
    else
        statementResult = true;

    return std::make_tuple(sintaxeCorrect, statementResult);
}

void Pre_Processamento::writeTokensToOutput()
{
    for(size_t i = 0; i < tokensList.size(); i++)
    {
        this->fileOutput << tokensList[i].nome << '\n';
    }
    this->fileOutput << std::endl;
}

//Toma a decisao se imprime a linha ou nao e ja imprime
void Pre_Processamento::printLine(std::string Line){
    std::stringstream linha(Line);
    std::string word;

    while (linha >> word){
        if (word == "equ"){
            //nao imprime a linha
            return;
        }
        if (word == "if"){
            this-> lastIF = true;
            //pego o rotulo associado ao if e verifico o estado dele
            linha >> word;
            for (size_t i = 0; i < rotulosList.size(); i++){
                //procura o rotulo associado ao if
                if (word == rotulosList[i].name){
                    if ((rotulosList[i].EquValue == 0) && (rotulosList[i].isEqu)){
                        this->lastIfState = false;
                    } else {
                        this-> lastIfState = true;
                    }
                }
            }
            return;
        }
    }

    if ((this->lastIF) && (!this->lastIfState)){
        //sginifica que a linha de cima era um EQU com valor 0, portanto nao imprime
        //atualiza a variavel lastIF
        this-> lastIF = false;
        return;

    }

    //Se chegar aqui eh pq essa linha deve ser impressa
    this-> fileOutput << Line << std::endl;
}

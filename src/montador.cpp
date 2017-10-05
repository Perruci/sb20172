#include "../include/montador.h"

// Contructor do montador
Montador::Montador(std::string inputFile, std::string outputFile)
{
    this->inputFileName =  inputFile;
    this->outputFileName = outputFile;
    this->fileText.open(inputFileName);
    this->fileOutput.open(outputFileName);
}
// Destruidor do montador
Montador::~Montador()
{
    // Sem preocupações com o close
    fileText.close();
    fileOutput.close();
}

/* Carrega instruções a partir da tabela de instruções */
void Montador::loadInstructions(std::string tablePath)
{
    /* Open and closes instruction table file */
    std::ifstream instructionTable(tablePath);
    //string de suporte para capturar os elementos
    std::string aux;
    //Loop para ler todas as linhas do codigo aberto e "tokenizar" elas (o token leva o : em consideracao ainda)
    while (getline(instructionTable, aux)){
        // char* ptr;  //ponteiro de suporte para o funcionamento do strlen dentro da funcao minuscula
        // ptr = &(aux[0]);  //ponteiro apontando pro primeiro caracter da string
        // aux = Montador::minuscula (ptr);

        std::stringstream lineStream (aux);
        Mnemonic instruction;
        /* Fill mnemonic with data from this line */
        std::string word;
        lineStream >> word;
        /* Set name */
        instruction.setName(word);
        lineStream >> word;
        /* Set noperands */
        instruction.setNOperands(atoi(word.c_str()));
        lineStream >> word;
        /* Set opcode */
        instruction.setOpcode(atoi(word.c_str()));
        lineStream >> word;
        /* Set size */
        instruction.setSize(atoi(word.c_str()));
        /* Push to instructionList */
        this->instructionList.push_back(instruction);
    }
    instructionTable.close();
}

/* Imprime Tokens guardades em tokensList */
void Montador::printIntstructions()
{
    if(this->instructionList.empty())
    {
        std::cout << "Instructions list empty" << '\n';
        return;
    }
    for(size_t i = 0; i < instructionList.size(); i++)
    {
        std::cout << "Instruction name: " << instructionList[i].nome      << " "
                  << "noperands: "        << instructionList[i].noperands << " "
                  << "opcode: "           << instructionList[i].opcode    << " "
                  << "size: "             << instructionList[i].size      << '\n';
    }
    std::cout << '\n';
}

// Função teste: extrair cada token do arquivo de entrada e escrevê-los de forma padrão na saída
void Montador::tokenizar()
{
    //string de suporte para capturar os tokens
    std::string aux;
    //Loop para ler todas as linhas do codigo aberto e "tokenizar" elas (o token leva o : em consideracao ainda)
    while (getline(this->fileText, aux)){
        char* ptr;  //ponteiro de suporte para o funcionamento do strlen dentro da funcao minuscula
        ptr = &(aux[0]);  //ponteiro apontando pro primeiro caracter da string
        aux = Montador::minuscula (ptr);


        std::stringstream lineStream (aux);
        Token token(aux);

        while (lineStream >> token.nome){
            this->tokensList.push_back(token);
        }
    }
}

/* Imprime Tokens guardades em tokensList */
void Montador::printTokens()
{
    if(this->tokensList.empty())
    {
        std::cout << "Token list empty" << '\n';
        return;
    }
    for(size_t i = 0; i < tokensList.size(); i++)
    {
        std::cout << tokensList[i].nome << " " << '\n';
    }
    std::cout << '\n';
}

/* Escreve um arquivo de saida a partir de tokensList */
void Montador::writeTokensToOutput()
{
    for(size_t i = 0; i < tokensList.size(); i++)
    {
        this->fileOutput << tokensList[i].nome << '\n';
    }
    this->fileOutput << std::endl;
}

//transforma toda a string em letras minusculas
std::string Montador::minuscula (char* aux){
    int tam = strlen (aux);

    for (int i = 0; i < tam; i++){
        aux[i] = (char) std::tolower(aux[i]);
    }

    return aux;
}

//Funcao que recebe o ponteiro pro inicio de uma string e trunca ela a partir do caracter indesejado, pode ser usada para ajeitar a extensao dos
//arquivos e para retirar o : dos rotulos (ex: teste.h -> teste  ou rotulo: -> rotulo)
std::string Montador::trunca_nome (char* nome, char indesejavel){
    std::string aux = nome;
    std::string aux2;
    int tam = strlen (nome); //numero de caracteres da string nome

    for (int i = 0; i < tam; i++){
        if(nome[i] == indesejavel){
            break;
        }
        aux2 += nome[i];
    }

    return aux2;
}

/* pre-processamento ----------------------------------------------------- */

std::string Montador::getOutputPrefix()
{
    std::string outputFile;
    //prepara o nome e abre o arquivo de saida
    char* ptr;
    ptr = &(this->outputFileName[0]);
    outputFile = Montador::trunca_nome(ptr,'.');
    outputFile += ".pre";

    return outputFile;
}

//Faz a etapa de pre-processamento e trata os EQU e IF, funcao criada sem utilizar a tokenizacao, pois estou com receio de que
//se fizermos a tokenizacao nessa parte estariamos violando a regra de passagem unica
bool Montador::pre_processamento(){
    bool check_section_text = false;  //passa a ser true quando acharmos a secao text
    int contador_tokens = 0;          //sera utilizado para navegar pela tokensList
    int contador_endereco = 0;        //sera utilizado para determinar o endereco de cada token do programa
    int haveRotuloInLine = 0;            //controle para caso aparecam dois rotulos na mesma linha
    int contador_de_linhas = 0;       //Controle para saber em qual linha do programa esta algum erro

    //prepara o nome e abre o arquivo de saida
    std::string OutputFile = getOutputPrefix();
    std::ofstream out_pre;
    out_pre.open (OutputFile);

    //Fecha e abre o arquivo do codigo para atualizar o ponteiro de arquivo, caso alguma outra funcao tenha usado ele
    this->reopenCodeFile();

    std::string aux;
    std::string token;

    //Loop para fazer o pre-processamento
    while (getline(this->fileText, aux)){
        char* ptr;  //ponteiro de suporte para o funcionamento do strlen dentro da funcao minuscula
        ptr = &(aux[0]);  //ponteiro apontando pro primeiro caracter da string
        aux = Montador::minuscula (ptr);

        contador_de_linhas++;               //incrementa a linha
        std::stringstream lineStream (aux);
        haveRotuloInLine = 0;               //prepara a variavel para  analisar o proximo rotulo

        //Loop para analisar cada token um a um 
        while (lineStream >> token){
            //coloca o token na lista
            this->tokensList.push_back(token);

            //se o token for um rotulo, trata ele
            if (tokensList[contador_tokens].isRotulo(token, aux, instructionList)){
                int tipo_rotulo = tokensList[contador_tokens].KindOfRotulo(token);
                
                //testa se eh uma declaracao de rotulo e se ja tem outra declaracao na mesma linha
                if (tipo_rotulo == 1){
                
                    //se ja tiver uma declaracao na linha sera erro
                    if (haveRotuloInLine == 1){
                        std::cout << "Mais de um Rotulo na linha " << contador_de_linhas << "\n";
                    }
                }
                Trata_rotulos(token, tipo_rotulo);
            }
            contador_tokens++;

            if (token == "text"){
                check_section_text = true;
            }

            if ((token == "equ")){
                
                }

           /* if (token == "if"){
                lineStream >> token; //pega o rotulo que vem depois do if
                for (int i = 0; i < rotulos.size(); i++){
                    //Se for verdade pega a linha debaixo e imprime ela no arquivo de saida
                    if ((token == rotulos[i]) && (true_or_false[i] != '0')){
                        getline(this->fileText, aux);
                        ptr = &(aux[0]);
                        aux = Montador::minuscula(ptr);
                        std::stringstream lineStream_aux2(aux);

                        while (lineStream_aux2 >> token){
                            out_pre << token << " ";
                        }
                    }
                    //Se o if representar uma mentira (rotulo = 0), nao imprime a linha debaixo no arquivo
                    if ((token == rotulos[i]) && (true_or_false[i] == '0')){
                        getline (this->fileText, aux);
                        std::stringstream lineStream_aux2(aux);

                        while (lineStream_aux2 >> token){
                            //do nothing
                        }
                    }
                }
            }*/
        }
    }
}

//checa a lista de rotulos procurando se determinado rotulo ja foi visto antes, retorno com o seguinte significado
// 0-> primeira vez que encontramos o rotulo
// 1xx-> rotulo ja encontrado e ja declarado (xx eh o indice da lista de rotulos onde esse rotulo se encontra)
// 2xx-> rotulo ja encontrado, mas ainda nao declarado (xx vide de cima)
int Montador::RotuloAlreadyFound(std::string token){
    //percorre a lista de rotulos procurando pelo rotulo
    for (int i = 0; i < rotulosList; i++){
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

void Montador::Trata_rotulos (){
    switch (tokensList[contador_tokens].isRotulo(token, aux, instructionList)){
        //nao eh rotulo
        case 0:
            break;

        //eh declaracao de um rotulo
        case 1:
            if (haveRotuloInLine == 1){
                std::cout << "Mais de um Rotulo na linha " << contador_de_linhas << "\n";
                return false;
            } else {
                haveRotuloInLine = 1;
                
                //atualiza a lista de rotulos da maneira correta (ver declaracao da funcao rotulo Alreadyfound pra entender)
                int flag_rotulo = Montador::RotuloAlreadyFound(token); 
                switch (flag % 100){
                    //primeira vez que encontramos esse rotulo
                    case 0:
                        rotulosList.push_back(token, true, contador_endereco);
                        break;
                    
                    //Ja encontramos esse rotulo e ele ja foi declarado
                    case 1:
                        //adiciona o endereco atual na lista de enderecos desse rotulo
                        rotulosList[flag-100].addList(contador_endereco);
                        break;  

                    //Ja encontramos esse rotulo, mas ele ainda nao foi declarado
                    case 2:
                        rotulosList[flag-200].address = contador_endereco;
                        rotulosList[flag-200].alreadyDeclared = true;
                }
                
            } 
        //eh rotulo, mas nao eh declaracao
        case 2:
            if
        
        
    }
}
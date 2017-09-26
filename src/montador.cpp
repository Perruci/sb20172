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

//Faz a etapa de pre-processamento e trata os EQU e IF, funcao criada sem utilizar a tokenizacao, pois estou com receio de que
//se fizermos a tokenizacao nessa parte estariamos violando a regra de passagem unica
void Montador::pre_processamento(){
    //int contador_equ = 0;   //conta quantos EQU tem no arquivo
    int check_section = 0;  //passa a ser 1 depois da primeira vez que aparecer a secao section pela primeira vez
    std::vector<std::string> rotulos;   //vetor contendo os nomes dos rotulos dos EQU's
    std::vector<char> true_or_false;   //vetor contendo se o rotulo de um EQU eh 1 ou 0, indice desse vetor indica o rotulo com o mesmo indice no vetor de rotulos
    std::string OutputFile;

    //prepara o nome e abre o arquivo de saida
    char* ptr;
    ptr = &(this->outputFileName[0]);
    OutputFile = Montador::trunca_nome(ptr,'.');
    OutputFile += ".pre";
    std::ofstream out_pre;
    out_pre.open (OutputFile);

    //Fecha e abre o arquivo do codigo para atualizar o ponteiro de arquivo, caso alguma outra funcao tenha usado ele
    this->fileText.close();
    this->fileText.open(this->inputFileName);

    std::string aux;
    std::string token;
    std::string token_aux;
    //Loop para fazer o pre-processamento
    while (getline(this->fileText, aux)){
        char* ptr;  //ponteiro de suporte para o funcionamento do strlen dentro da funcao minuscula
        ptr = &(aux[0]);  //ponteiro apontando pro primeiro caracter da string
        aux = Montador::minuscula (ptr);

        std::stringstream lineStream (aux);
        std::stringstream lineStream_aux (aux);
        lineStream_aux >> token_aux;
        ptr = &(token_aux[0]);
        token_aux = trunca_nome(ptr,':');
        
        while (lineStream >> token){
            if (token == "section"){
                check_section = 1;
            }

            if ((token == "equ") && (check_section == 0)){
                rotulos.push_back(token_aux);
                lineStream >> token;
                char help = token[0];
                true_or_false.push_back(help);
                break;
            }

            if (token == "if"){
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
            }
        }

        out_pre << "\n";
    }


}
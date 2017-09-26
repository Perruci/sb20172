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
        char* ptr;
        ptr = &(aux[0]);
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

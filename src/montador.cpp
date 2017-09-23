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

        std::stringstream lineStream (aux);
        Token token(aux);

        while (lineStream >> token.nome){
            this->fileOutput << token.nome << " ";
        }

        this->fileOutput << std::endl;
    }
}

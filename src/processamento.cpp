#include "../include/processamento.h"

Processamento::Processamento(std::string inputFile, std::string outputFile)
{
    this->inputFileName =  inputFile;
    this->outputFileName = outputFile;
    this->setInputExtension(".pre");
    this->setOutputExtension (".mcr");
    this->fileText.open(inputFileName);
    this->fileOutput.open(outputFileName);
}

Processamento::~Processamento()
{
    // Sem preocupações com o close
    fileText.close();
    fileOutput.close();
}

std::string Processamento::setOutputExtension(std::string extension)
{
    this->outputFileName = string_ops::setOutputExtension(inputFileName, extension);

    return outputFileName;
}

std::string Processamento::setInputExtension(std::string extension)
{
    this->inputFileName = string_ops::setOutputExtension(inputFileName, extension);

    return inputFileName;
}

//Metodo principal que vai realizar o processamento
bool Processamento::run(){
    
    std::string line;
    std::string word;

    //loop linha a linha para analisar o arquivo de entrada
    while (getline(this->fileText, line)){
        std::stringstream lineStream(line);

        //loop palavra a palavra para procurar as macros
        while (lineStream >> word){
            //caso ache uma macro, a palavra antes dela eh o seu rotulo
            if (word == "macro"){

            }
        }
    }
    return true;
}
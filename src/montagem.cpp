#include "../include/montagem.h"

Montagem::Montagem(std::string inputFile, std::string outputFile)
{
    this->inputFileName =  inputFile;
    this->outputFileName = outputFile;
    this->setInputExtension(".mcr");
    this->setOutputExtension (".o");
    this->fileText.open(inputFileName);
    this->fileOutput.open(outputFileName);
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
    this-> fileOutput << "line" << std::endl;
    return true;
}


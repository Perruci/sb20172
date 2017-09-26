#ifndef _MONTADOR_H_
#define _MONTADOR_H_

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <string.h>
#include <vector>

#include "token.h"

// Declaracao da classe Montador
// Esta classe abriga os métodos desenvolvidos para traduzir e montar o programa
class Montador
{
private:
    std::string inputFileName;
    std::string outputFileName;

    std::vector<Token> tokensList;

    std::ifstream fileText;
    std::ofstream fileOutput;
public:
    Montador(std::string, std::string);
    ~Montador();
    void tokenizar();
    void printTokens();
    std::string minuscula (char*);
    void writeTokensToOutput();
    void pre_processamento();
    void processamento();
    void montagem();
};
#endif

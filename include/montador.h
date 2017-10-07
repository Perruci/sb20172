#ifndef _MONTADOR_H_
#define _MONTADOR_H_

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <string.h>
#include <vector>

#include "pre_processamento.h"
#include "token.h"
#include "mnemonic.h"
#include "rotulo.h"

// Declaracao da classe Montador
// Esta classe abriga os métodos desenvolvidos para traduzir e montar o programa
class Montador
{
private:
    std::string inputFileName;
    std::string outputFileName;

    std::vector<Mnemonic> instructionList;

    Pre_Processamento* _pre_process;
public:
    Montador(std::string inputFile, std::string outputFile, std::string tablePath = "tables/instructions.txt");
    ~Montador();
    /* Mnemonic */
    void loadInstructions(std::string);
    void printIntstructions();
    /* Tokens */
    void tokenizar();
    void printTokens();
    void writeTokensToOutput();
    /* pre-processamento */
    bool pre_processamento();
    /* Processamento */
    void processamento();
    /* Montagem */
    void montagem();
};
#endif

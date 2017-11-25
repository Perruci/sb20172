#ifndef _MONTADOR_H_
#define _MONTADOR_H_

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <string.h>
#include <vector>

#include "pre_processamento.h"
#include "processamento.h"
#include "token.h"
#include "mnemonic.h"
#include "rotulo.h"
#include "montagem.h"

// Declaracao da classe Montador
// Esta classe abriga os métodos desenvolvidos para traduzir e montar o programa
class Montador
{
private:
    std::string inputFileName;
    std::string outputFileName;

    std::vector<Mnemonic> instructionList;

    Pre_Processamento* _pre_process;
    Processamento* _processamento;
    Montagem* _montagem;
public:
    Montador(std::string inputFile, std::string outputFile, std::string tablePath = "tables/instructions.txt");
    ~Montador();
    /* Mnemonic */
    void loadInstructions(std::string);
    void printIntstructions();
    /* pre-processamento */
    bool pre_processamento(int);
    /* Processamento */
    bool processamento();
    /* Montagem */
    bool montagem(int);

    bool run(int);
};
#endif

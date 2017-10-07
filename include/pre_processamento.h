#ifndef _PRE_PROCESSAMENTO_H_
#define _PRE_PROCESSAMENTO_H_

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <string.h>
#include <vector>

#include "token.h"
#include "mnemonic.h"
#include "rotulo.h"
#include "string_ops.h"

// Declaracao da classe Montador
// Esta classe abriga os métodos desenvolvidos para traduzir e montar o programa
class Pre_Processamento
{
private:
    std::string inputFileName;
    std::string outputFileName;
    std::ifstream fileText;
    std::ofstream fileOutput;

    std::vector<Token> tokensList;
    std::vector<Mnemonic> instructionList;
    std::vector<Rotulo> rotulosList;

    //Fecha e abre o arquivo do codigo para atualizar o ponteiro de arquivo, caso alguma outra funcao tenha usado ele
    inline bool reopenCodeFile()
    {
        this->fileText.close();
        this->fileText.open(this->inputFileName);
        return(fileText.is_open());
    };
public:
    Pre_Processamento(std::string, std::string, std::vector<Mnemonic>&);
    ~Pre_Processamento();
    bool run();
    /* Utilidades */
    std::string setOutputExtension(std::string);
    /* Rotulos */
    void Trata_rotulos (std::string, int, int);
    int RotuloAlreadyFound(std::string);
    void printRotulos();
};
#endif

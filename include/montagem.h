#ifndef _MONTAGEM_H_
#define _MONTAGEM_H_

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <string.h>
#include <vector>
#include <tuple>

#include "token.h"
#include "mnemonic.h"
#include "rotulo.h"
#include "string_ops.h"
#include "macro.h"

class Montagem
{
private:
    std::string inputFileName;
    std::string outputFileName;
    std::ifstream fileText;
    std::ofstream fileOutput;

    std::vector<Mnemonic> instructionList;
    std::vector<Rotulo> rotulosList;
    std::vector<Token> tokensList;
    
    //lista de inteiros para preparar o arquivo objeto em memoria antes de imprimir no arquivo
    std::vector<int> outputFileList;


    //Fecha e abre o arquivo do codigo para atualizar o ponteiro de arquivo, caso alguma outra funcao tenha usado ele
    inline bool reopenCodeFile()
    {
        this->fileText.close();
        this->fileText.open(this->inputFileName);
        return(fileText.is_open());
    };

public:
    Montagem(std::string, std::string, std::vector<Mnemonic>&);
    ~Montagem();

    bool run();

    /* Rotulos */
    void trata_rotulos (std::string, int, int);
    void declaracao_de_rotulo(std::string, int);
    void chamada_de_rotulo(std::string, int);
    void printRotulos();

     /*Erros Lexicos*/
     bool scannerLexico (std::string, char);

     /*Instrucoes*/
     bool isInstruction(Token);
     int InstructionOperand(Token);
     int instructionOpcode(Token);

    std::string setOutputExtension(std::string);
    std::string setInputExtension(std::string);
};
#endif
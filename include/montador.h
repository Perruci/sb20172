#ifndef _MONTADOR_H_
#define _MONTADOR_H_

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <string.h>
#include <vector>

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

    std::vector<Token> tokensList;
    std::vector<Mnemonic> instructionList;
    std::vector<Rotulo> rotulosList;

    std::ifstream fileText;
    std::ofstream fileOutput;

    //Fecha e abre o arquivo do codigo para atualizar o ponteiro de arquivo, caso alguma outra funcao tenha usado ele
    inline bool reopenCodeFile()
    {
        this->fileText.close();
        this->fileText.open(this->inputFileName);
        return(fileText.is_open());
    };
public:
    Montador(std::string, std::string);
    ~Montador();
    /* Mnemonic */
    void loadInstructions(std::string);
    void printIntstructions();
    /* Tokens */
    void tokenizar();
    void printTokens();
    std::string minuscula (char*);
    std::string trunca_nome (char*, char);
    void writeTokensToOutput();
    /* pre-processamento */
    bool pre_processamento();
    std::string getOutputPrefix();

    void processamento();
    void montagem();
};
#endif

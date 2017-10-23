#ifndef _PRE_PROCESSAMENTO_H_
#define _PRE_PROCESSAMENTO_H_

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

    int contador_de_linhas;
    int contador_endereco;
    std::vector<int> address_adjusts;
    void update_address_adjusts();

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
    std::vector<int> get_addresses_adjusts();
    /* Utilidades */
    std::string setOutputExtension(std::string);
    std::string treat_comments(std::string);
    std::string check_equ_call(std::string, std::string);
    /* Rotulos */
    void trata_rotulos (std::string, int, int);
    void declaracao_de_rotulo(std::string, int);
    void chamada_de_rotulo(std::string, int);
    int RotuloAlreadyFound(std::string);
    void printRotulos();
    /* EQU */
    bool trata_equ(std::string, size_t);
    /* IF */
    std::tuple<bool, bool> trata_if(std::string, size_t);
    bool lastIF;
    bool lastIfState;

    void writeTokensToOutput();
    void printLine(std::string line);
};
#endif

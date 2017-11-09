#ifndef _PROCESSAMENTO_H_
#define _PROCESSAMENTO_H_

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

class Processamento
{
private:
    std::string inputFileName;
    std::string outputFileName;
    std::ifstream fileText;
    std::ofstream fileOutput;

    std::vector <Macro> macrosList;

    int contador_de_linhas;
    int contador_endereco;
    std::vector<int> previous_adjusts;
    std::vector<int> address_adjusts;
    void update_address_adjusts();
    void update_address_macros(Macro);

public:
    Processamento(std::string, std::string);
    ~Processamento();

    bool run(std::vector<int>);

    std::vector<int> get_addresses_adjusts();
    int get_address();
    /*Macros*/
    void printMacros();
    std::string getCode ();
    std::string getRotulo(std::string);

    void printLineToOutput(std::string);

    std::string setOutputExtension(std::string);
    std::string setInputExtension(std::string);
};
#endif

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

public:
    Processamento(std::string, std::string);
    ~Processamento();

    bool run();

    /*Macros*/
    void printMacros();
    std::string getCode ();
    std::string getRotulo(std::string);

    std::string setOutputExtension(std::string);
    std::string setInputExtension(std::string);
};
#endif

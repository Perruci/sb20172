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

class Processamento
{
private:
    std::string inputFileName;
    std::string outputFileName;
    std::ifstream fileText;
    std::ofstream fileOutput;

public:
    Processamento(std::string, std::string);
    ~Processamento();

    bool run();

    std::string setOutputExtension(std::string);
    std::string setInputExtension(std::string);
};
#endif

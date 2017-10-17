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

public:
    Montagem(std::string, std::string);
    ~Montagem();

    bool run();

    std::string setOutputExtension(std::string);
    std::string setInputExtension(std::string);
};
#endif
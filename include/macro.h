#ifndef _MACRO_H_
#define _MACRO_H_

#include <iostream>
#include <string.h>
#include <vector>
#include <sstream>

//Declaracao da classe Macro
class Macro
{
    public:
        std::string name;
        std::string codigo;

        Macro(std::string, std::string);
};
#endif

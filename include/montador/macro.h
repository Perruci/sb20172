#ifndef _MACRO_H_
#define _MACRO_H_

#include <iostream>
#include <string.h>
#include <vector>
#include <algorithm>
#include <sstream>

//Declaracao da classe Macro
class Macro
{
    public:
        std::string name;
        std::string codigo;
        int address;
        size_t num_lines;

        Macro(std::string, std::string, int);
        size_t get_numlines();
        int get_address();
        void set_address(int);
};
#endif

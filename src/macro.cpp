#include "../include/macro.h"


//construtor da classe Macro
Macro::Macro(std::string nome, std::string code, int address){
    this->name = nome;
    this-> codigo = code;
    set_address(address);
}

size_t Macro::get_numlines()
{
    std::string str = this->codigo;
    return std::count(str.begin(), str.end(), '\n');
}

void Macro::set_address(int addr)
{
    this->address = addr;
}

int Macro::get_address()
{
    return this->address;
}

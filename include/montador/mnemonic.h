#ifndef _MNEMONIC_H_
#define _MNEMONIC_H_

#include <iostream>
#include <string.h>

// Tipos de Mnemonicos: instruções do assempbly inventado
class Mnemonic
{
    public:
        Mnemonic();
        unsigned int opcode;
        unsigned int noperands;
        unsigned int size;
        std::string nome;
        inline void setName(std::string word){this->nome = word;};
        inline void setOpcode(unsigned int opcode){this->opcode = opcode;};
        inline void setSize(unsigned int size){this->size = size;};
        inline void setNOperands(unsigned int noperands){this->noperands = noperands;};
};
#endif

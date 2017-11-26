#ifndef _CARREGADOR_H_
#define _CARREGADOR_H_

#include "../montador/mnemonic.h"
#include "../mem_sim/mem_space.h"
#include "../mem_sim/mem_chunk.h"

class Carregador
{
private:
    std::vector<Mnemonic> instructionList;
public:
    Carregador();
    ~Carregador();
    void loadInstructions(std::string tablePath = "tables/instructions.txt");
    void printIntstructions();
};

#endif

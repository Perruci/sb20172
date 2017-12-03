#ifndef _EXEC_SIM_H_
#define _EXEC_SIM_H_

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <string.h>
#include <vector>

#include "../mem_sim/mem_space.h"
#include "../../montador/mnemonic.h"

class ExecutionSimulator
{
private:
    /* Mnemonic */
    std::vector<Mnemonic> instructionList; // Instruction list for decoding
    int identifyInstruction(unsigned int);
    void printIntstructions();
    void loadInstructions(std::string tablePath = "tables/instructions.txt");
public:
    ExecutionSimulator();
    void simulate(std::vector<MemorySpace>);
};

#endif

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
    std::vector<MemorySpace> object_memory;
    /* Register */
    int reg_accum;
    std::string last_instruction;
    std::vector<int> arguments_list;
    /* Mnemonic */
    std::vector<Mnemonic> instructionList; // Instruction list for decoding
    int identifyInstruction(unsigned int);
    void printIntstructions();
    void loadInstructions(std::string tablePath = "tables/instructions.txt");
    size_t processInstruction(std::vector<MemorySpace>, size_t);
public:
    ExecutionSimulator();
    void simulate(std::vector<MemorySpace>);
};

#endif

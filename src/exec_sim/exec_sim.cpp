#include "../include/carregador/exec_sim/exec_sim.h"

ExecutionSimulator::ExecutionSimulator()
{
    this->loadInstructions();
}

void ExecutionSimulator::simulate(std::vector<MemorySpace> object_memory)
{
    bool foundStop = false;
    unsigned int expected_args = 0;
    size_t program_counter = 0;
    while(program_counter < object_memory.size())
    {
        int value = object_memory[program_counter].get_value();
        /* instruction processing */
        // get index for instruction. -1 for not instruction
        int idx = this->identifyInstruction(value);
        // If has found the stop instruction, its surely a data declaration
        if(foundStop)
        {
            std::cout << value << "\t" << "Data" <<'\n';
        }
        // If the opcode is not identified, or an argument is expected, is an argument
        else if ((idx == -1) | (expected_args > 0))
        {
            expected_args = expected_args - 1;
            std::cout << value << "\t" << "Argument" <<'\n';
        }
        // If neither, must be an instruction
        else
        {
            std::cout << value << "\t" << this->instructionList[idx].nome <<'\n';
            expected_args = this->instructionList[idx].noperands;
            // If instruction is stop, update found stop
            if(this->instructionList[idx].nome == "stop")
            {
                foundStop = true;
            }
        }
        program_counter++;
    }

}

int ExecutionSimulator::identifyInstruction(unsigned int value)
{
    for(unsigned int idx = 0; idx < this->instructionList.size(); idx++)
    {
        if(value == this->instructionList[idx].opcode)
            return idx;
    }
    return -1;
}

void ExecutionSimulator::loadInstructions(std::string tablePath)
{
    /* Open and closes instruction table file */
    std::ifstream instructionTable(tablePath);
    //string de suporte para capturar os elementos
    std::string aux;
    while (getline(instructionTable, aux)){
        std::stringstream lineStream (aux);
        Mnemonic instruction;
        /* Fill mnemonic with data from this line */
        std::string word;
        lineStream >> word;
        /* Set name */
        instruction.setName(word);
        lineStream >> word;
        /* Set noperands */
        instruction.setNOperands(atoi(word.c_str()));
        lineStream >> word;
        /* Set opcode */
        instruction.setOpcode(atoi(word.c_str()));
        lineStream >> word;
        /* Set size */
        instruction.setSize(atoi(word.c_str()));
        /* Push to instructionList */
        this->instructionList.push_back(instruction);
    }
    // this->printIntstructions();
}

/* Imprime Tokens guardades em tokensList */
void ExecutionSimulator::printIntstructions()
{
    if(this->instructionList.empty())
    {
        std::cout << "Instructions list empty" << '\n';
        return;
    }
    for(size_t i = 0; i < instructionList.size(); i++)
    {
        std::cout << "Instruction name: " << instructionList[i].nome      << " "
                  << "noperands: "        << instructionList[i].noperands << " "
                  << "opcode: "           << instructionList[i].opcode    << " "
                  << "size: "             << instructionList[i].size      << '\n';
    }
    std::cout << '\n';
}

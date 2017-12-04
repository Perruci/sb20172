#include "../include/carregador/exec_sim/exec_sim.h"

ExecutionSimulator::ExecutionSimulator()
{
    this->reg_accum = 0;
    this->loadInstructions();
}

void ExecutionSimulator::simulate(std::vector<MemorySpace> obj_memory)
{
    this->object_memory = obj_memory;
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
            // std::cout << value << "\t" << "Data" <<'\n';
        }
        // If the opcode is not identified, or an argument is expected, is an argument
        else
        {
            if ((idx == -1) | (expected_args > 0))
            {
                expected_args = expected_args - 1;
                // std::cout << value << "\t" << "Argument" <<'\n';
                arguments_list.push_back(value);
            }
            // If neither, must be an instruction
            else
            {
                // std::cout << value << "\t" << this->instructionList[idx].nome <<'\n';
                this->last_instruction = this->instructionList[idx].nome;
                expected_args = this->instructionList[idx].noperands;
                // If instruction is stop, update found stop
                if(this->instructionList[idx].nome == "stop")
                {
                    foundStop = true;
                }
            }
            // If its the last expected argument, execute instruction
            if(expected_args == 0)
                program_counter = this->processInstruction(object_memory, program_counter);
        }
        program_counter++;
    }

}

/*
    Executa instrução identificada por last_instruction com argumentos arguments_list
 */
size_t ExecutionSimulator::processInstruction(std::vector<MemorySpace> obejct_memory, size_t program_counter)
{
    size_t ret_address = program_counter;
    if(this->last_instruction == "add")
    {
        std::cout << "Add: " << this->reg_accum << " + " << obejct_memory[arguments_list[0]].load() << '\n';
        this->reg_accum += obejct_memory[arguments_list[0]].load();
    }
    if(this->last_instruction == "sub")
    {
        std::cout << "Sub: " << this->reg_accum << " - " << obejct_memory[arguments_list[0]].load() << '\n';
        this->reg_accum -= obejct_memory[arguments_list[0]].load();
    }

    if(this->last_instruction == "mult")
    {
        std::cout << "Mult: " << this->reg_accum << " * " << obejct_memory[arguments_list[0]].load() << '\n';
        this->reg_accum *= obejct_memory[arguments_list[0]].load();
    }

    if(this->last_instruction == "div")
    {
        std::cout << "Div: " << this->reg_accum << " / " << obejct_memory[arguments_list[0]].load() << '\n';
        this->reg_accum /= obejct_memory[arguments_list[0]].load();
    }

    if(this->last_instruction == "jmp")
    {
        std::cout << "Jump to " << this->arguments_list[0] << '\n';
        ret_address = arguments_list[0] - 1; // adjust for program_counter--
    }

    if(this->last_instruction == "jmpn")
    {
        std::cout << "Jump if 0 > " << this->reg_accum << " to " << this->arguments_list[0] << '\n';
        if(this->reg_accum < 0)
            ret_address = arguments_list[0] - 1; // adjust for program_counter--
    }

    if(this->last_instruction == "jmpp")
    {
        std::cout << "Jump if 0 < " << this->reg_accum << " to " << this->arguments_list[0] << '\n';
        if(this->reg_accum > 0)
            ret_address = arguments_list[0] - 1; // adjust for program_counter--
    }

    if(this->last_instruction == "jmpz")
    {
        std::cout << "Jump if 0 == " << this->reg_accum << " to " << this->arguments_list[0] << '\n';
        if(this->reg_accum == 0)
            ret_address = arguments_list[0] - 1; // adjust for program_counter--
    }

    if(this->last_instruction == "copy")
    {
        std::cout << "Copy: [" << this->arguments_list[0] << "] [" << this->arguments_list[1] << "]" << '\n';
        obejct_memory[arguments_list[0]].store(object_memory[arguments_list[1]].load());
    }

    if(this->last_instruction == "load")
    {
        std::cout << "Load: " << "Accum <- " << object_memory[arguments_list[0]].load() << '\n';
        this->reg_accum = object_memory[arguments_list[0]].load();
    }

    if(this->last_instruction == "store")
    {
        std::cout << "Store: [" <<  this->arguments_list[0] << "] <- Accum = " << this->reg_accum << '\n';
        object_memory[this->arguments_list[0]].store(this->reg_accum);
    }

    if(this->last_instruction == "input")
    {
        std::cout << "[" << this->arguments_list[0] << "] <- Input" << '\n';
        int input;
        std::cin >> input;
        object_memory[this->arguments_list[0]].store(input);
    }

    if(this->last_instruction == "output")
    {
        std::cout << "Output <- " << "[" << this->arguments_list[0] << "]" << '\n';
        std::cout << this->object_memory[this->arguments_list[0]].load() << '\n';
    }

    if(this->last_instruction == "stop")
        std::cout << "Stop! " << '\n';

    arguments_list.clear();
    return ret_address;
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

#include "../../include/carregador/carregador.h"

Carregador::Carregador(int argc, char* argv[])
{
    this->setFileNames(argv);
    this->loadInstructions();
    this->openIOFiles();
    this->objectChunkCreated = false;
}

Carregador::~Carregador()
{
    // Sem preocupações com o close
    fileObject.close();
    fileOutput.close();
    if(objectChunkCreated)
        delete this->objectChunk;
}

void Carregador::openIOFiles()
{
    this->fileObject.open(objFileName);
    this->fileOutput.open(outputFileName);
}

bool Carregador::fitChunks(MemorySimulator& mem_sim)
{
    if(mem_sim.freeMemorySize() < this->totalChunkSize())
    {
        std::cout << "OUT OF MEMORY - YOUR PROGRAM WILL NOT BE LOADED" << '\n';
        return false;
    }
    // Try to assign to memory
    if(!mem_sim.assignToMemory(this->objectChunk))
    {
        std::cout << "OUT OF MEMORY - YOUR PROGRAM WILL NOT BE LOADED" << '\n';
        return false;
    }
    return true;
}

void Carregador::processObjectFile()
{
    std::string word;
    bool foundStop = false;
    unsigned int expected_args = 0;
    unsigned int current_address = 0;
    while (this->fileObject >> word)
    {
        // Get integer opcode
        int value = std::stoi(word);
        // Add instruction to buffer
        this->addToBuffer(current_address, value);
        // Update address
        current_address++;
        /* instruction processing */
        // get index for instruction, -1 for not instruction
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
    }
    this->createObjectChunk();
    std::cout << "Object file chunk Print!" << '\n';
    this->print_objectChunk();
}

void Carregador::addToBuffer(int address, int value)
{
    MemorySpace mem_space(address, value);
    this->memoryBuffer.push_back(mem_space);
}

/* Creates a minimal objectChunk based on what is stored in memoryBuffer.
Then resets the memory buffer */
void Carregador::createObjectChunk()
{
    assignChunk(this->memoryBuffer.size());
    this->memoryBuffer.clear();
}

/* Assign a new objectChunk  based on its size */
bool Carregador::assignChunk(int size)
{
    // Create auxiliar chunk variable () always starts on 0
    this->objectChunk = new MemoryChunk(0, size);
    // Assign memoryBuffer to aux_chunk
    bool no_overflow = this->objectChunk->assign(this->memoryBuffer);
    // Detect overflow
    if(!no_overflow)
        std::cout << "Error, carregador internal chunk out of memory" << '\n';
    return no_overflow;
}

int Carregador::totalChunkSize()
{
    return this->objectChunk->get_size();
}

void Carregador::print_objectChunk()
{
    std::cout << "Object file chunk:" << '\n';
    this->objectChunk->print();
}

int Carregador::identifyInstruction(unsigned int value)
{
    for(unsigned int idx = 0; idx < this->instructionList.size(); idx++)
    {
        if(value == this->instructionList[idx].opcode)
            return idx;
    }
    return -1;
}

void Carregador::setFileNames(char* argv[])
{
    this->objFileName = std::string(argv[1]);
    this->outputFileName = string_ops::setOutputExtension(this->objFileName, ".im");
    std::cout << this->outputFileName << '\n';
}

void Carregador::loadInstructions(std::string tablePath)
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
void Carregador::printIntstructions()
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

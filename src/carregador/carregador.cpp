#include "../../include/carregador/carregador.h"

Carregador::Carregador(int argc, char* argv[])
{
    this->setFileNames(argv);
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
    unsigned int current_address = 0;
    while (this->fileObject >> word)
    {
        // Get integer opcode
        int value = std::stoi(word);
        // Add instruction to buffer
        this->addToBuffer(current_address, value);
        // Update address
        current_address++;
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
    this->objectChunkCreated = true;
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

void Carregador::setFileNames(char* argv[])
{
    this->objFileName = std::string(argv[1]);
    this->outputFileName = string_ops::setOutputExtension(this->objFileName, ".im");
    std::cout << this->outputFileName << '\n';
}

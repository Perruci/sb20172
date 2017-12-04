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
    if(!mem_sim.assignToMemory(this->objectChunk, this->relativeAddresses))
    {
        std::cout << "OUT OF MEMORY - YOUR PROGRAM WILL NOT BE LOADED" << '\n';
        return false;
    }
    return true;
}

void Carregador::set_bitmapRelatives(std::string bitmapString)
{
    for(size_t bit_count = 0; bit_count < bitmapString.size(); bit_count++)
    {
        if(bitmapString[bit_count] == '1')
        {
            this->relativeAddresses.push_back(bit_count);
        }
    }
}

void Carregador::processHeaders()
{
    /*
        Process Headers
        Composed of tree lines:
        H: File_Name
        H: [file_size](int)
        H: [relatives bitmap]
     */
    std::string word;
    std::string line;
    size_t line_count = 0;
    while(line_count < 3 && getline(this->fileObject, line))
    {
        std::stringstream lineStream (line);
        lineStream >> word; // get label
        if(word != "H:")
            std::cout << "[Carregador] Unexpected Header format: " << word  << '\n';
        lineStream >> word;
        if(line_count == 0)
        {
            this->executableName = word; // get executable name
            std::cout << "[Carregador] Executable name: " << this->executableName << '\n';
        }
        if(line_count == 1)
        {
            this->executableSize = std::stoi(word); // get executableSize
            std::cout << "[Carregador] Executable size: " << this->executableSize << '\n';
        }
        if(line_count == 2)
        {
            this->set_bitmapRelatives(word);
            std::cout << "Relative addresses: " << '\n';
            for(size_t idx = 0; idx < this->relativeAddresses.size(); idx++)
                std::cout << this->relativeAddresses[idx] << '\n';
        }
        line_count++;
    }
    // Continue to process object file
}

void Carregador::processObjectFile()
{
    this->processHeaders();

    /*
        Process Text Section
     */
    std::string line;
    getline(this->fileObject, line);
    std::cout << "Text line: "<< line << '\n';
    // Iterate on each word
    std::stringstream lineStream (line);
    std::string word;
    lineStream >> word; // get label
    unsigned int current_address = 0;
    while (lineStream >> word)
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

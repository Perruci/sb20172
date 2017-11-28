#include "../../include/carregador/carregador.h"

Carregador::Carregador(int argc, char* argv[])
{
    this->setFileNames(argv);
    this->loadInstructions();
    this->openIOFiles();
    this->processObjectFile();
}

Carregador::~Carregador()
{
    // Sem preocupações com o close
    fileObject.close();
    fileOutput.close();
}

void Carregador::openIOFiles()
{
    this->fileObject.open(objFileName);
    this->fileOutput.open(outputFileName);
}

void Carregador::processObjectFile()
{
    std::string word;
    bool foundStop = false;
    unsigned int expected_args = 0;
    unsigned int current_address = 0;
    while (this->fileObject >> word)
    {
        int value = std::stoi(word);
        // get index for instruction, -1 for not instruction
        int idx = this->identifyInstruction(value);
        // If has found the stop instruction, its surely a data declaration
        if(foundStop)
        {
            std::cout << value << "\t" << "Data" <<'\n';
        }
        // If the opcode is not identified, or an argument is expected, is an argument
        else if (idx == -1 | expected_args > 0)
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
                foundStop = true;
        }
        this->addToBuffer(current_address, value);
        current_address++;
    }
    this->assignChunk(current_address);
}

void Carregador::addToBuffer(int address, int value)
{
    MemorySpace mem_space(address, value);
    this->memoryBuffer.push_back(mem_space);
}

bool Carregador::assignChunk(int size)
{
    MemoryChunk aux_chunk(0, size);
    bool no_overflow = aux_chunk.assign(this->memoryBuffer);
    if(no_overflow)
        aux_chunk.print();
    else
        std::cout << "Error, internal chunk out of memory" << '\n';
    this->subChunks.push_back(aux_chunk);
    return no_overflow;
}

int Carregador::identifyInstruction(int value)
{
    for(int idx = 0; idx < this->instructionList.size(); idx++)
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

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
    std::string bin_value;
    while (this->fileObject >> bin_value)
    {
        std::cout << bin_value << '\n';
    }
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

#include "../include/montador.h"

// Contructor do montador
Montador::Montador(std::string inputFile, std::string outputFile, std::string tablePath)
{
    this->inputFileName =  inputFile;
    this->outputFileName = outputFile;
    this->loadInstructions(tablePath);
    this->_pre_process = new Pre_Processamento(inputFileName, outputFileName, instructionList);
    this->_processamento = new Processamento(inputFileName, outputFileName);
    this->_montagem = new Montagem(inputFileName, outputFileName);
}
// Destruidor do montador
Montador::~Montador()
{
    delete _pre_process;
}

/* Carrega instruções a partir da tabela de instruções */
void Montador::loadInstructions(std::string tablePath)
{
    /* Open and closes instruction table file */
    std::ifstream instructionTable(tablePath);
    //string de suporte para capturar os elementos
    std::string aux;
    //Loop para ler todas as linhas do codigo aberto e "tokenizar" elas (o token leva o : em consideracao ainda)
    while (getline(instructionTable, aux)){
        // char* ptr;  //ponteiro de suporte para o funcionamento do strlen dentro da funcao minuscula
        // ptr = &(aux[0]);  //ponteiro apontando pro primeiro caracter da string
        // aux = Montador::minuscula (ptr);

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
    instructionTable.close();
}

/* Imprime Tokens guardades em tokensList */
void Montador::printIntstructions()
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

/* pre-processamento ----------------------------------------------------- */
bool Montador::pre_processamento()
{
    return this->_pre_process->run();
}

/* processamento ----------------------------------------------------- */
bool Montador::processamento()
{
    return this->_processamento->run();
}

/* montagem ----------------------------------------------------------- */
bool Montador::montagem()
{
    return this->_montagem->run();
}
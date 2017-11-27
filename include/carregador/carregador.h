#ifndef _CARREGADOR_H_
#define _CARREGADOR_H_

#include "../montador/string_ops.h"
#include "../montador/mnemonic.h"
#include "../mem_sim/mem_space.h"
#include "../mem_sim/mem_chunk.h"

class Carregador
{
private:
    std::string objFileName;
    std::string outputFileName;
    std::ifstream fileObject;
    std::ofstream fileOutput;
    std::vector<Mnemonic> instructionList;
public:
    Carregador(int, char**);
    ~Carregador();
    void openIOFiles();
    void processObjectFile();
    void setFileNames(char**);
    void loadInstructions(std::string tablePath = "tables/instructions.txt");
    void printIntstructions();
};

#endif

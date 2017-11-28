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
    std::vector<Mnemonic> instructionList; // Instruction list for decoding
    std::vector<MemorySpace> memoryBuffer; // Memory Spaces buffer to create subChunks
    std::vector<MemoryChunk> subChunks;    // List of minimal chunks for deployment
    void createSubChunk();
public:
    Carregador(int, char**);
    ~Carregador();
    void openIOFiles();
    void processObjectFile();
    /* MemorySpace */
    void addToBuffer(int, int);
    /* MemoryChunk */
    bool assignChunk(int);
    void print_subChunks();
    /* Mnemonic */
    int identifyInstruction(int);
    void printIntstructions();
    /* Generic functions */
    void setFileNames(char**);
    void loadInstructions(std::string tablePath = "tables/instructions.txt");
};

#endif

#ifndef _CARREGADOR_H_
#define _CARREGADOR_H_

#include "../montador/string_ops.h"
#include "mem_sim/mem_space.h"
#include "mem_sim/mem_chunk.h"
#include "mem_sim/mem_sim.h"

class Carregador
{
private:
    std::string objFileName;
    std::string outputFileName;
    std::ifstream fileObject;
    std::ofstream fileOutput;
    std::vector<MemorySpace> memoryBuffer; // Memory Spaces buffer to create objectChunk
    MemoryChunk* objectChunk;    // Object file chunk
    bool objectChunkCreated;
    void openIOFiles();
    /* MemorySpace */
    void addToBuffer(int, int);
    /* MemoryChunk */
    void createObjectChunk();
    bool assignChunk(int);
    int totalChunkSize();
    void print_objectChunk();

    /* Generic functions */
    void setFileNames(char**);
public:
    Carregador(int, char**);
    ~Carregador();
    void processObjectFile();
    bool fitChunks(MemorySimulator&);
    inline std::vector<MemorySpace> getMemoryInstructions(){return this->objectChunk->getMemoryPile();};
};

#endif

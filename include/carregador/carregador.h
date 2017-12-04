#ifndef _CARREGADOR_H_
#define _CARREGADOR_H_

#include "../montador/string_ops.h"
#include "mem_sim/mem_space.h"
#include "mem_sim/mem_chunk.h"
#include "mem_sim/mem_sim.h"

class Carregador
{
private:
    std::string executableName;
    int executableSize;
    std::string objFileName;
    std::string outputFileName;
    std::ifstream fileObject;
    std::vector<MemorySpace> memoryBuffer; // Memory Spaces buffer to create objectChunk
    MemoryChunk* objectChunk;    // Object file chunk
    bool objectChunkCreated;
    /* File setup */
    void openIOFiles();
    void set_bitmapRelatives(std::string);
    std::vector<int> relativeAddresses;
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
    void processHeaders();
    void processObjectFile();
    bool fitChunks(MemorySimulator&);
    inline std::string getOutputFileName(){return this->outputFileName;};
    inline std::vector<MemorySpace> getMemoryInstructions(){return this->objectChunk->getMemoryPile();};
};

#endif

#ifndef _MEMORY_SIM_H_
#define _MEMORY_SIM_H_

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <string.h>
#include <vector>

#include "mem_space.h"
#include "mem_chunk.h"

class MemorySimulator
{
private:
    /* Arguments setup */
    bool process_arguments(int, char**);
    bool set_num_chunks(char*);
    bool set_chunk_sizes(char**);
    bool set_chunk_addresses(char**);
    void print_chunk_sizes();
    void print_chunk_addresses();
    int num_chunks;
    std::vector<int> chunk_sizes;
    std::vector<int> chunk_addresses;
    /* Memory Chunks */
    void initMemoryChunks();
    std::vector<MemoryChunk> memory_chunk_pile;
public:
    MemorySimulator();
    ~MemorySimulator();

    bool init(int, char**);
    int freeMemorySize();
    bool assignToMemory(MemoryChunk*);
    void print_chunks();
};

#endif

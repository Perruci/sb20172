#ifndef _MEMORY_CHUNK_H_
#define _MEMORY_CHUNK_H_

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <string.h>
#include <vector>

#include "mem_space.h"

/* Header only class to represent Memory Chunks */
class MemoryChunk
{
private:
    int initial_address;
    int size;
    std::vector<MemorySpace> memory_pile;
    /* Initialize memory_pile Memory Spaces with zero value */
    inline void initMemoryPile()
    {
        auto start_address  = this->initial_address;
        auto end_address    = start_address + this->size;
        for(auto adress = start_address; adress < end_address; adress++)
        {
            MemorySpace mem_space(adress, 0);
            memory_pile.push_back(mem_space);
        }
    };
public:
    inline MemoryChunk(int address, int size)
    {
        this->initial_address = address;
        this->size = size;
        this->initMemoryPile();
    };
    inline void print() const
    {
        // Loop through each element and print it out
        for(auto const mem_space : this->memory_pile)
            std::cout << mem_space.get_address() << " " << mem_space.load() << '\n';
    }
    inline int get_size() const{return this->size;};
};

#endif
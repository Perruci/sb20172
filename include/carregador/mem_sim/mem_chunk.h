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
    unsigned int initial_address;
    unsigned int size;
    int freeSize;
    std::vector<MemorySpace> memory_pile;
    /* Initialize memory_pile Memory Spaces with zero value */
    inline void initMemoryPile()
    {
        unsigned int start_address  = this->initial_address;
        unsigned int end_address    = start_address + this->size;
        for(unsigned int adress = start_address; adress < end_address; adress++)
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
        this->freeSize = size;
        this->initMemoryPile();
    };
    /* Assign a vector of memory spaces into the chunk */
    inline bool assign(std::vector<MemorySpace> mem_buffer)
    {
        if(mem_buffer.size() > this->size)
            return false;
        for(unsigned int idx = 0; idx < mem_buffer.size(); idx++)
        {
            this->memory_pile[idx] = mem_buffer[idx];
            this->freeSize -= 1;
        }
        return true;
    };
    /* Assign values from a vector of memory spaces */
    inline bool assignValues(std::vector<MemorySpace> mem_buffer)
    {
        if(mem_buffer.size() > this->size)
            return false;
        for(unsigned int idx = 0; idx < mem_buffer.size(); idx++)
        {
            this->memory_pile[idx].store(mem_buffer[idx].load());
            this->freeSize -= 1;
        }
        return true;
    };
    /*
        Ajust relative addresses according to a vector of relative indexes and initial_address
     */
    inline void adjustRelatives(std::vector<int> relative_idx)
    {
        for(size_t idx = 0; idx < relative_idx.size(); idx++)
        {
            int adjustedValue = this->memory_pile[idx].load() +  this->initial_address;
            this->memory_pile[idx].store(adjustedValue);
        }
    };
    inline void print() const
    {
        // Loop through each element and print it out
        for(auto const mem_space : this->memory_pile)
            std::cout << mem_space.get_address() << " " << mem_space.load() << '\n';
    }
    inline int get_initial_address() const {return this->initial_address;};
    inline int get_size() const{return this->size;};
    inline int getFreeSpace() const{return this->freeSize;};
    inline std::vector<MemorySpace> getMemoryPile(){return this->memory_pile;};
};

#endif

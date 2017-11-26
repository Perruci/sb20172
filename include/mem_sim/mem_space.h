#ifndef _MEMORY_SPACE_H_
#define _MEMORY_SPACE_H_

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <string.h>
#include <vector>

/* Header only class to represent a memory space */
class MemorySpace
{
private:
    inline void set_address(int address){this->address = address;};
    inline void set_value(int value)    {this->value = value;};
public:
    inline MemorySpace(int address, int value)
    {
        this->set_address(address);
        this->set_value(value);
    };
    inline int load()                   const {return this->value;};
    inline void store(int value)        {this->set_value(value);};
    inline int get_address()            const {return this->address;};

    int address;
    int value;
};

#endif

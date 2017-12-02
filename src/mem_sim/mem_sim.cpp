#include "../../include/mem_sim/mem_sim.h"

MemorySimulator::MemorySimulator()
{

}

MemorySimulator::~MemorySimulator()
{

}

bool MemorySimulator::init(int argc, char* argv[])
{
    return this->process_arguments(argc, argv);
}

bool MemorySimulator::process_arguments(int argc, char* argv[])
{
    if(argc < 4)
    {
        std::cout << "Too few arguments on execution" << '\n';
        return false;
    }
    if(!set_num_chunks(argv[2]))
        return false;

    if(!set_chunk_sizes(argv))
        return false;

    if(!set_chunk_addresses(argv))
        return false;

    this->initMemoryChunks();

    // this->print_chunks();

    return true;
}

void MemorySimulator::initMemoryChunks()
{
    for(auto idx = 0; idx < this->num_chunks; idx++)
    {
        MemoryChunk chunk(this->chunk_addresses[idx], this->chunk_sizes[idx]);
        this->memory_chunk_pile.push_back(chunk);
    }
}

int MemorySimulator::freeMemorySize()
{
    int freeSpace = 0;
    for(auto idx = 0; idx < this->num_chunks; idx++)
    {
        freeSpace += this->memory_chunk_pile[idx].getFreeSpace();
    }
    return freeSpace;
}

bool MemorySimulator::assignToMemory(MemoryChunk* memory_request_)
{
    return true;
}

/* Arguments Processing ------------------------------------ */
bool MemorySimulator::set_num_chunks(char* c_str)
{
    // Expected input: ./carregador prog1 [num_chunks] ...
    if(!c_str)
    {
        std::cout << "Expected number of chunks, recieved Null" << '\n';
        return false;
    }
    // converts to std::sting
    std::string str(c_str);
    this->num_chunks = std::stoi(str);
    return true;
}

bool MemorySimulator::set_chunk_sizes(char* argv[])
{
    // Converts function parameters to integer chunk_sizes
    // Expected input: ./carregador prog1 [num_chunks] [chunk_sizes]*num_chunks ...
    // Loop begining in the forth parameter, until num_chunks+3
    auto start = 3;      // end of chunk sizes
    auto end = start + this->num_chunks;    // end of chunk addresses
    for(auto idx = start; idx < end; idx++)
    {
        // check for existance
        if(!argv[idx])
        {
            std::cout << "Expected " << this->num_chunks << " chunk sizes, but recieved " << idx-3 << '\n';
            return false;
        }
        std::string str(argv[idx]);
        this->chunk_sizes.push_back(std::stoi(str));
    }
    return true;
}

bool MemorySimulator::set_chunk_addresses(char* argv[])
{
    // Converts function parameters to integer chunk_addresses
    // Expected input: ./carregador prog1 [num_chunks] [chunk_sizes]*num_chunks [chunk_addresses]*num_chunks
    // Loop begining in num_chunks+2, until 2*num_chunks+2
    auto start = this->num_chunks + 3;      // end of chunk sizes
    auto end = start + this->num_chunks;    // end of chunk addresses
    for(auto idx = start; idx < end; idx++)
    {
        // check for existance
        if(!argv[idx])
        {
            std::cout << "Expected " << this->num_chunks << " chunk addresses, but recieved " << idx-start << '\n';
            return false;
        }
        std::string str(argv[idx]);
        this->chunk_addresses.push_back(std::stoi(str));
    }
    return true;
}

void MemorySimulator::print_chunk_sizes()
{
    // Loop through each element and print it out
    for(auto const size_i : this->chunk_sizes)
    {
        std::cout << size_i << '\n';
    }
}

void MemorySimulator::print_chunk_addresses()
{
    // Loop through each element and print it out
    for(auto const addr_i : this->chunk_addresses)
    {
        std::cout << addr_i << '\n';
    }
}


void MemorySimulator::print_chunks()
{
    // Loop through each element and print it out
    for(auto idx = 0; idx < this->num_chunks; idx++)
    {
        std::cout << "Chunk " << idx+1 << ", Size: " << this->memory_chunk_pile[idx].get_size() <<'\n';
        this->memory_chunk_pile[idx].print();
    }
}

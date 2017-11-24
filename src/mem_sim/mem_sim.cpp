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

bool MemorySimulator::set_num_chunks(char* c_str)
{
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
    // Loop begining in the third parameter, until num_chunks+2
    for(size_t i = 2; i < this->num_chunks+2; i++)
    {
        // check for existance
        if(!argv[i])
        {
            std::cout << "Expected " << this->num_chunks << " chunk sizes, but recieved " << i << '\n';
            return false;
        }
        std::string str(argv[i]);
        this->chunk_sizes.push_back(std::stoi(str));
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

bool MemorySimulator::process_arguments(int argc, char* argv[])
{
    if(argc < 3)
    {
        std::cout << "Too few arguments on execution" << '\n';
        return false;
    }
    if(!set_num_chunks(argv[1]))
        return false;

    if(!set_chunk_sizes(argv))
        return false;

    std::cout << "num_chunks: " << this->num_chunks << '\n';
    this->print_chunk_sizes();

    return true;
}

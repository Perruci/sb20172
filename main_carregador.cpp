#include "include/mem_sim/mem_sim.h"
#include "include/carregador/carregador.h"

int main(int argc, char* argv[])
{
    Carregador carregador(argc, argv);
    MemorySimulator sim(argc, argv);
    return 0;
}

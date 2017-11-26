#include "include/mem_sim/mem_sim.h"
#include "include/carregador/carregador.h"

int main(int argc, char* argv[])
{
    MemorySimulator sim;
    sim.init(argc, argv);
    Carregador carregador;
    return 0;
}

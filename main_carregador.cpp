#include "include/carregador/carregador.h"
#include "include/carregador/mem_sim/mem_sim.h"

int main(int argc, char* argv[])
{
    Carregador carregador(argc, argv);
    MemorySimulator sim;
    if(!sim.init(argc, argv))
        return EXIT_FAILURE;
    carregador.processObjectFile();
    if(!carregador.fitChunks(sim))
        return EXIT_FAILURE;
    sim.print_chunks();
    return EXIT_SUCCESS;
}

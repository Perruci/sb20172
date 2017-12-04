#include "include/carregador/carregador.h"
#include "include/carregador/mem_sim/mem_sim.h"
#include "include/carregador/exec_sim/exec_sim.h"

int main(int argc, char* argv[])
{
    Carregador carregador(argc, argv);
    MemorySimulator mem_sim;
    ExecutionSimulator exec_sim;
    if(!mem_sim.init(argc, argv))
        return EXIT_FAILURE;
    carregador.processObjectFile();
    if(!carregador.fitChunks(mem_sim))
        return EXIT_FAILURE;
    mem_sim.writeMemoryToFile(carregador.getOutputFileName());
    // Run simulation
    exec_sim.simulate(carregador.getMemoryInstructions());
    return EXIT_SUCCESS;
}

#include "../../include/ligador/ligador.h"

Ligador::Ligador(int argc, char* argv[])
{
    this->processArguments(argc, argv);
}

Ligador::~Ligador()
{

}

bool Ligador::processArguments(int argc, char* argv[])
{
    if(argc < 2)
    {
        std::cout << "[Ligador] não foi recebido um arquivo objeto." << '\n';
        return false;
    }
    for(int arg_idx = 0; arg_idx < argc; arg_idx++)
    {
        std::string str(argv[arg_idx]); // get each filename
        this->nomes.push_back(str); // add to nomes vector
    }
    return true;
}

#include "../../include/ligador/ligador.h"

Ligador::Ligador(int argc, char* argv[])
{
    this->processArguments(argc, argv);
}

Ligador::~Ligador()
{

}

bool Ligador::run()
{
    std::cout << "Ligador run!" << '\n';
    for(size_t i = 1; i < this->nomes.size(); i++)
        this->processFile(i);
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

bool Ligador::processFile(size_t file_idx)
{
    if(file_idx > this->nomes.size())
    {
        std::cout << "[Ligador][processFile] file_idx inválido" << '\n';
        return false;
    }
    std::cout << "Reading file: " << this->nomes[file_idx] << '\n';
    // Open filestream
    this->fileInput.open(this->nomes[file_idx]);
    std::string line;
    while(getline(this->fileInput, line))
    {
        std::cout << line << '\n';
    }
    this->fileInput.close();
    return true;
}

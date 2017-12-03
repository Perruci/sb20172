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
    if(argc > 4){
        std::cout << "[Ligador] foram inseridos mais arquivos de entrada que o limite do ligado." << '\n';
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
    std::string word;
    int contador_de_linhas = 0;
    
    while(getline(this->fileInput, line))
    {
        contador_de_linhas++;
        //Primeira linha so tem o nome, ja pegamos ele antes entao podemos pular ela
        if(contador_de_linhas == 1){
            continue;
        }
        std::stringstream linestream (line);
        
        //Vamos iterar a linha analisando palavra a palavra
        while(linestream >> word){
            //Se for cabeçalho, temos:
            if(word == "H:"){
                //Se estivermos na segunda linha significa que temos o tamanho do nosso arquivo
                if(contador_de_linhas == 2){
                    linestream >> word; //bota o tamanho na word
                    int tam = std::stoi (word,nullptr);
                    this->tamanhos.push_back(tam);
                }

                //Se estivermos na terceira linha significa que temos o mapa de bits do nosso arquivo
                if(contador_de_linhas == 3){
                    linestream >> word; //bota o mapa de bits da string word
                    for(size_t i = 0; i < word.size(); i++){
                        int bit = word[i] - 0x30; //faz a conversão de char pra int na forma bruta
                        this->mapa_de_bits.push_back(bit);
                    }
                }

                //Pegamos a proxima word e analisamos se ela é tabela de definicoes ou tabela de uso
                linestream >> word;
                if(word == "TU:"){

                }

                if(word == "TD:"){

                }
            }

            //Se for arquivo, temos:
            if(word == "T:"){

            }
        }
        
        std::cout << line << '\n';
    }
    this->fileInput.close();
    return true;
}

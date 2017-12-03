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
    for(size_t i = 1; i < this->nomes.size(); i++){
        this->processFile(i);
    }
    this->printRotulos();
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
        bool rotuloExistente = false;
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
                    //pega a próxima string que será o nome do rotulo
                    linestream >> word;
                    //Checa se já existe esse rotulo na lista de rotulos
                    for(size_t i = 0; i < this->rotulosList.size(); i++){
                        if (word == rotulosList[i].name){
                            rotuloExistente = true;
                            //Continua pegando as proximas palavras dessa word, pois elas sao os endereços onde
                            while(linestream >> word){
                                int endereco = std::stoi(word,nullptr);
                                //Atualiza o endereço dependendo do modulo
                                if(file_idx == 1){
                                    endereco = endereco;
                                } else if (file_idx == 2){
                                    endereco = endereco + this->tamanhos[0];
                                } else {
                                    endereco = endereco + this->tamanhos[0] + this->tamanhos[1];
                                }
                                rotulosList[i].addressList.push_back(endereco);
                            }
                        }
                    }

                    //Se ainda nao existir um rotulo com esse nome, criamos agora
                    if(!rotuloExistente){
                        rotulo aux;
                        aux.name = word;
                        while(linestream >> word){
                            int endereco = std::stoi(word,nullptr);
                            //Atualiza o endereço dependendo do modulo
                            if(file_idx == 1){
                                    endereco = endereco;
                                } else if (file_idx == 2){
                                    endereco = endereco + this->tamanhos[0];
                                } else {
                                    endereco = endereco + this->tamanhos[0] + this->tamanhos[1];
                                }
                            aux.addressList.push_back(endereco);
                        }
                        this->rotulosList.push_back(aux);
                    }

                }

                if(word == "TD:"){
                    //pega a próxima string que será o nome do rotulo
                    linestream >> word;
                    //Checa se já existe esse rotulo na lista de rotulos
                    for(size_t i = 0; i < this->rotulosList.size(); i++){
                        if (word == rotulosList[i].name){
                            rotuloExistente = true;
                            //Continua pegando as proximas palavras dessa word, pois elas sao os endereços onde
                            while(linestream >> word){
                                int endereco = std::stoi(word,nullptr);
                                //Atualiza o endereço dependendo do modulo
                                if(file_idx == 1){
                                    rotulosList[i].address = endereco;
                                } else if (file_idx == 2){
                                    rotulosList[i].address = endereco + this->tamanhos[0];
                                } else {
                                    rotulosList[i].address = endereco + this->tamanhos[0] + this->tamanhos[1];
                                }
                            }
                            this->rotulosList[i].modulo = file_idx;
                        }
                    }

                    //Se ainda nao existir um rotulo com esse nome, criamos agora
                    if(!rotuloExistente){
                        rotulo aux;
                        aux.name = word;
                        aux.modulo = file_idx;
                        while(linestream >> word){
                            int endereco = std::stoi(word,nullptr);
                            //Atualiza o endereço dependendo do modulo
                            endereco = endereco + this->tamanhos[file_idx - 2];
                            if(file_idx == 1){
                                    aux.address = endereco;
                                } else if (file_idx == 2){
                                    aux.address = endereco + this->tamanhos[0];
                                } else {
                                    aux.address = endereco + this->tamanhos[0] + this->tamanhos[1];
                                }
                        }
                        this->rotulosList.push_back(aux);
                    }
                }

            //Se for arquivo, temos:
            if(word == "T:"){

            }
        }
        }
        //std::cout << line << '\n';
        
    }
    this->fileInput.close();
    return true;
}

void Ligador::printRotulos(){
    std::cout << "Os tamahnos dos modulos são ";
    for(size_t i = 0; i < this->tamanhos.size(); i++){
        std::cout << tamanhos[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "O mapa de bits final é ";
    for(size_t i = 0; i < this->mapa_de_bits.size(); i++){
        std::cout << this->mapa_de_bits[i];
    }
    std::cout<<std::endl;
    for(size_t i = 0; i < this->rotulosList.size(); i++){
        std::cout << "Nome: " << this->rotulosList[i].name << std::endl;
        std::cout << "Endereço: " << this->rotulosList[i].address << std::endl;
        std::cout << "Modulo: " << this->rotulosList[i].modulo << std::endl;
        std::cout << "Endereços de uso: ";
        for(size_t j = 0; j < this->rotulosList[i].addressList.size(); j++){
            std::cout << this->rotulosList[i].addressList[j] << " ";
        }
        std::cout << std::endl << std::endl;
    }
}
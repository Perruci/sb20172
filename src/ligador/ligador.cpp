#include "../../include/ligador/ligador.h"

Ligador::Ligador(int argc, char* argv[])
{
    this->processArguments(argc, argv);
    this->quantArgs = argc;
}

Ligador::~Ligador()
{

}

void Ligador::run()
{
    // std::cout << "Ligador run!" << '\n';
    for(size_t i = 1; i < this->nomes.size(); i++){
        this->processFile(i);
    }
    //this->printRotulos();
    this->applyCorrection();
    this->printOutput();
    //this->printRotulos();
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
    // std::cout << "Reading file: " << this->nomes[file_idx] << '\n';

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
            }

            //Se for arquivo, temos:
            if(word == "T:"){
                //vai pegando as proximas words e colocando elas na saida final
                while(linestream >> word){
                    int numero = std::stoi(word,nullptr);
                    this->outputList.push_back(numero);
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
    std::cout << "A saida por enquanto é ";
    for(size_t i = 0; i < this->outputList.size(); i++){
        std::cout << this->outputList[i] << " ";
    }
    std::cout<< std::endl;
}

void Ligador::applyCorrection(){
    bool EnderecoDoRotulo = false;
    int position = 0;
    for(size_t i = 0; i < this->outputList.size(); i++){
        EnderecoDoRotulo = false;
        //So altera o valor se o mapa de bits apontar que naquele endereco tem um valor realocavel
        if(this->mapa_de_bits[i] == 1){
            //Percorre a lista de rotulos para ver se algum dos rotulos eh utilizado naquele endereco
            for(size_t j = 0; j < this->rotulosList.size(); j++){
                //percorre a lista de enderecos dos rotulos
                for(size_t k = 0; k < this-> rotulosList[j].addressList.size(); k++){
                    if(this->rotulosList[j].addressList[k] == position){
                        EnderecoDoRotulo = true;
                        this->outputList[i] = this->outputList[i] + this->rotulosList[j].address;
                    }
                }
            }
            //Se nao for endereco de uso de nenhum rotulo, atualiza com o fator de correcao do modulo
            if(!EnderecoDoRotulo){
                //Se for um endereço que ainda esta no primeiro modulo não soma nada
                if(position < this->tamanhos[0]){

                }
                //Se for um endereço do segundo modulo, aplica o fator de correçao
                if((this->quantArgs > 2) && (position >= this->tamanhos[0]) && (position < (this->tamanhos[0] + this->tamanhos[1]))){
                    this->outputList[i] = this->outputList[i] + this->tamanhos[0];
                }
                //Se for um endereço do terceiro modulo, aplica o fator de correçao
                if((this->quantArgs > 3) && (position >= (this->tamanhos[0] + this->tamanhos[1])) && (position < (this->tamanhos[0] + this->tamanhos[1] + this->tamanhos[2]))){
                    this->outputList[i] = this->outputList[i] + this->tamanhos[0] + this->tamanhos[1];
                }
            }
        }
        position++;
    }
    //std::cout << "A quantidade de argumentos é " << this->quantArgs << std::endl;
}

void Ligador::printOutput(){
    std::string nome = string_ops::tira_path(this->nomes[1]);
    this->fileOutput.open(nome);
    int tam = 0;

    for(size_t i = 0; i < this->tamanhos.size(); i++){
        tam = tam + this->tamanhos[i];
    }

    this->fileOutput << "H: " << nome << std::endl;
    this->fileOutput << "H: " << tam << std::endl;
    this->fileOutput << "H: ";
    for(size_t i = 0; i < this->mapa_de_bits.size(); i++){
        this->fileOutput << this->mapa_de_bits[i];
    }
    this->fileOutput << std::endl;
    this->fileOutput << "T: ";
    for (size_t i = 0; i < this->outputList.size(); i++){
        this->fileOutput << this->outputList[i] << " ";
    }
}

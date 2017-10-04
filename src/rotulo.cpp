#include "../include/rotulo.h"

//construtor
Rotulo::Rotulo(std::string nome, bool flag, int inicio){
    this->name = nome;
    this->alreadyDeclared = flag;
    
    if (flag){
        this->address = inicio;
    } else {
        this->address = -1;
    }
}

//Caso o rotulo ja tenha sido identificado, mas ainda nao tenhamos encontrado o seu endereco incial
//ATENCAO: se esse metodo precisar ser chamado, significa que ja vimos esse rotulo antes, ou seja, precisamos tratar
//os enderecos onde ele ja foi visto
void Rotulo::setState(int inicial){
    //se ja tiver sido encontrado o endereco, nao faz nada (teste para protecao)
    if (this->alreadyDeclared){
        return;
    }

    //atualiza o estado e seta o endereco inicial
    this->alreadyDeclared = true;
    this->address = inicial;
    return;
}
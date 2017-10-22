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

bool Rotulo::setEQU(std::string line)
{
    // Ultimo rótulo recebe flag EQU
    this->isEqu = true;

    // encontra a posicao do EQU
    size_t equ_pos = line.find("equ");
    if(equ_pos == std::string::npos)
    {
        std::cout << "Syntax error at EQU" << '\n';
        return false;
    }
    // cria uma substring a partir de equ até o final
    std::string substring = line.substr(equ_pos + 3);
    // tratamento de exeção
    try
    {
        // extrai-se um valor inteiro e recebe sua posição seguinte
        size_t pos_int;
        int value = std::stoi(substring, &pos_int);
        std::cout << "EQU value: " << value << '\n';
        this->EquValue = value;
    }
    catch (const std::invalid_argument& e)
    {
        std::cout << "Invalid EQU: " << substring << std::endl;
        return false;
    }
    catch (const std::out_of_range& e)
    {
        std::cout << "EQU out of range: " << substring << std::endl;
        return false;
    }
    return true;
}

std::string Rotulo::getEQU_str()
{
    return std::to_string(this->EquValue);
}

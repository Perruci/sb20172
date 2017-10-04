#ifndef _Rotulo_H_
#define _Rotulo_H_

#include <iostream>
#include <string.h>
#include <vector>

// Rotulos(labels) do programa
class Rotulo
{
    public:
        Rotulo(std::string, bool, int); //recebe o nome, se ele ja foi declarado e o endereco caso ele tenha sido declarado
        int address; //onde ele foi declarado, pra onde ele "aponta", se nao tiver sido declarado ainda -> igual a -1
        bool alreadyDeclared; //se ele ja foi declarado ou apenas chamado por uma instrucao
        std::string name;
        std::vector<int> addressList; //lista dos enderecos onde ele foi utilizado
        bool isEqu;
        bool EquValue;

        inline void addList(int endereco){this->addressList.push_back(endereco);};
        void setState(int inicial);
};
#endif
#ifndef _Rotulo_H_
#define _Rotulo_H_

#include <iostream>
#include <string.h>
#include <vector>

namespace tipo_rotulo
{
    enum
    {
        declaracao,
        chamada
    };
};

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
        int EquValue;
        bool isMem;             //caso o rotulo indique uma diretiva e nao um endereco de pulo

         //Variaveis importantes para a análise de arquivos modularizados
        bool isExtern = false;
        bool isPublic = false;
        bool isBegin = false;

        //Nao sei se eh o melhor jeito, mas e como pensei em fazer agora
        bool isConst = false;
        int constValue;
        bool isSpace = false;
        int spaceQuantity = 1;

        inline void addList(int endereco){this->addressList.push_back(endereco);};
        void setState(int inicial);
        bool setEQU(std::string);
        std::string getEQU_str();
};
#endif

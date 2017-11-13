#ifndef _STRING_OPS_
#define _STRING_OPS_

#include <algorithm>
#include <string>
#include <string.h>

/* Utilidades */
namespace string_ops
{
    //Funcao que recebe uma string e trunca ela a partir do caracter indesejado, pode ser usada para ajeitar a extensao dos
    //arquivos e para retirar o : dos rotulos (ex: teste.h -> teste  ou rotulo: -> rotulo)
    inline std::string trunca_nome (std::string nome, char indesejavel)
    {
        size_t indesejavel_index = nome.find_last_of(indesejavel);
        std::string novoNome = nome.substr(0, indesejavel_index);

        return novoNome;
    }

    inline std::string setOutputExtension(std::string fileName, std::string extension)
    {
        fileName = string_ops::trunca_nome(fileName,'.');
        fileName += extension;

        return fileName;
    }

    inline std::string minuscula (std::string str)
    {
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);
        return str;
    }

    inline bool isAsmFile (char* arg){
        int tam = strlen(arg);
        int n = tam - 4;
        std::string terminacao;
        for(n;n < tam; n++){
            terminacao = terminacao + arg[n];
        }

        std::cout << "A terminacao do segundo argumento eh " << terminacao << std::endl;

        if(terminacao == ".asm"){
            return true;
        }

        return false;
    }
}

#endif

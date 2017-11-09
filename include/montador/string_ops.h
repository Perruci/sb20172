#ifndef _STRING_OPS_
#define _STRING_OPS_

#include <algorithm>
#include <string>

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

}

#endif

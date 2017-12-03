#ifndef _STRING_OPS_
#define _STRING_OPS_

#include <algorithm>
#include <string>
#include <string.h>
#include <iostream>
#include <stdio.h>

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
        int tam = strlen(arg);  //pega a qtd de caracteres que tem na string
        std::string terminacao; // cria uma string de suporte para armazenar a terminacao da string original

        //Pega os 4 ultimos caracteres da string original e salva na string terminacao
        for(int n = tam - 4 ; n < tam ; n++){
            terminacao = terminacao + arg[n];
        }

        //Se a string representar um arquivo asm
        if(terminacao == ".asm"){
            return true;
        }

        return false;
    }

    inline void deleteUnusedFiles(std::string fileName){
        
        //deleta o arquivo com extensão .mcr
        fileName = string_ops::setOutputExtension(fileName, ".mcr");
        remove(fileName.c_str());

        //deleta o arquivo com extensão .pre
        fileName = string_ops::setOutputExtension(fileName, ".pre");
        remove(fileName.c_str());

    }

    inline std::string tira_path(std::string FileName){
        std::size_t pos = FileName.rfind('/');
        std::string nome = FileName.substr(pos + 1);

        nome = string_ops::trunca_nome(nome, '.');

        return nome;
    }
}

#endif

#ifndef _LIGADOR_H_
#define _LIGADOR_H_

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

#include "../montador/string_ops.h"

class Ligador
{
private:
    std::vector<std::string> nomes;                 //vetor de strings contendo o nome de cada um dos arquivos de entrada, em ordem de entrada
    std::vector<int> tamamnhos;                     // ¨ ¨ ¨ ¨ inteiros ¨ ¨ os tamanhos ¨ ¨ ¨ ¨ ¨ ¨ ¨ ¨ ¨ ¨ ¨ ¨ ¨ ¨ ¨ ¨ ¨ ¨ ¨ ¨ ¨ ¨ ¨ ¨ ¨ ¨ ¨
    std::vector<int> mb1;                           //mapa de bits do primeiro arquivo
    std::vector<int> mb2;                           //¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨ segundo ¨¨¨¨¨¨¨¨
    std::vector<int> mb3;                           //¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨ terceiro¨¨¨¨¨¨¨¨

    bool processArguments(int, char**);
public:
    Ligador(int, char**);
    ~Ligador();

};

#endif

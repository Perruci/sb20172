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
    std::vector<int> tamanhos;                     // ¨ ¨ ¨ ¨ inteiros ¨ ¨ os tamanhos ¨ ¨ ¨ ¨ ¨ ¨ ¨ ¨ ¨ ¨ ¨ ¨ ¨ ¨ ¨ ¨ ¨ ¨ ¨ ¨ ¨ ¨ ¨ ¨ ¨ ¨ ¨
    std::vector<int> mapa_de_bits;                  //mapa de bits de todos os arquivos juntos 
    std::vector<int> mb1;                           //mapa de bits do primeiro arquivo
    std::vector<int> mb2;                           //¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨ segundo ¨¨¨¨¨¨¨¨
    std::vector<int> mb3;                           //¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨ terceiro¨¨¨¨¨¨¨¨
    std::ifstream fileInput;
    std::ofstream fileOutput;

    std::vector<int> outputList;                    //vetor que contem a lista de saida

    bool processArguments(int, char**);
    bool processFile(size_t);
public:
    Ligador(int, char**);
    ~Ligador();
    bool run();

};

#endif

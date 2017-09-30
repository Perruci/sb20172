#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <string.h>

#include "include/montador.h"
#include "include/token.h"

using namespace std;

int main (int argc, char* argv[]){

    //Requisito do professor que sejam sempre passsados 3 argumentos de entrada
    //que somados com o arquivo executado dao 4 argumentos no total.
    if (argc != 4){
         cout << "ERRO: Foi passado o numero errado de argumentos de entrada\n";
        return 0;
    }

    //o argv[1] contem o argumento de operacao (ex:-m), como so queremos a letra sem o '-', estou pegando o char do endereco deslocado de 1 byte
    char operation = *(argv[1]+1);

    //Pega o nome do arquivo de entrada onde esta o codigo a ser analisado
    string inputFile = argv[2];

    //Pega o nome do arquivo de saida que sera criado pelo nosso codigo
    string outputFile = argv[3];


    //Cria o objeto montador e ja tokeniza o arquivo de entrada
    Montador montador(inputFile, outputFile);
    montador.loadInstructions("tables/instructions.txt");
    montador.printIntstructions();

    switch (operation){
        //Operação de preprocessamento, coloca a estensão “.pre” no arquivo e somente
        //avalia as diretivas EQU e IF.
        case 'p':
            montador.pre_processamento();
            cout << "Definir algo para fazer a operacao de preprocessamento\n";
            break;

        //Operação de processamento de macros, coloca a estensão “.mcr” no arquivo e somente avalia
        //as diretivas EQU e IF e substitue as MACROS.
        case 'm':
            //pre-procesasmento (arq_entrada, arq_saida);
            //procesasmento (arq_saida);
            cout << "Definir algo para fazer a operacao de processamento\n";
            break;

        //Operação de motagem, coloca a estensão “.o” realiza a montagem de programa usando o PROCESSO DE PASSAGEM ÚNICA.
        case 'o':
            //pre-procesasmento (arq_entrada, arq_saida);
            //procesasmento (arq_saida);
            //montador (arq_saida);
            cout << "Definir algo para fazer a operacao de montagem\n";
            break;

        //So entrara no caso default se for uma operacao nao esperada, portanto significa que e erro
        default:
            cout << "ERRO: O argumento " << argv[1] << " passado, nao condiz com nenhum modo de operacao\n";
            break;
    }

    //montador.printTokens();

    //montador.writeTokensToOutput();

    return 0;
}

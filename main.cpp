#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <string.h>

#include "include/montador/montador.h"
#include "include/montador/token.h"

using namespace std;

void trabalho_1(int argc, char* argv[]){
    // Trabalho 1!
    char operation;  //Vai definir o modo de operacao do programa
    operation = *(argv[1]+1);
    //Pega o nome do arquivo de entrada onde esta o codigo a ser analisado
    string inputFile = argv[2];

    //Pega o nome do arquivo de saida que sera criado pelo nosso codigo
    string outputFile = argv[3];


    //Cria o objeto montador e ja tokeniza o arquivo de entrada
    Montador montador(inputFile, outputFile);
    //montador.printIntstructions();

    switch (operation){
        //Operação de preprocessamento, coloca a estensão “.pre” no arquivo e somente
        //avalia as diretivas EQU e IF.
        case 'p':
            montador.pre_processamento();
            break;

        //Operação de processamento de macros, coloca a estensão “.mcr” no arquivo e somente avalia
        //as diretivas EQU e IF e substitue as MACROS.
        case 'm':
            montador.pre_processamento();
            montador.processamento();
            break;

        //Operação de motagem, coloca a estensão “.o” realiza a montagem de programa usando o PROCESSO DE PASSAGEM ÚNICA.
        case 'o':
            montador.pre_processamento();
            montador.processamento();
            montador.montagem ();
            break;
        //So entrara no caso default se for uma operacao nao esperada, portanto significa que e erro
        default:
            cout << "ERRO: O argumento " << argv[1] << " passado, nao condiz com nenhum modo de operacao e nem eh um arquivo .asm\n";
            break;
        }
    return;
    }

void trabalho_2(int argc, char* argv[]){
    // Trabalho 2!
    //Vamos percorrer os argumentos do programa fazendo o processo de montagem deles
    //Mas se tiver so 1 arquivo .asm vamos fazer diferente pq nesse caso ele nao pode ter begin e end
    if(argc == 2){
        string inputFile = argv[1]; //pega o nome do arquivo de entrada
        string outputFile = string_ops::trunca_nome(inputFile, '.');   //Trunca o nome do arquivo de entrada no . para termos o radicao do arquivo de saida
        Montador montador(inputFile, outputFile);
        montador.pre_processamento();
        montador.montagem(); //Precisamos passar um argumento aqui para fazer a analise sem poder ter begin e end
    } else{
        //Caso tenha mais de 1 arquivo .asm devemos fazer o pre-processamento e a montagem de todos
        for(int i = 1; i < argc ; i++){
            string inputFile = argv[i];
            string outputFile = string_ops::trunca_nome(inputFile,'.');
            Montador montador(inputFile, outputFile);
            montador.pre_processamento();
            montador.montagem(); //Precisamos passar um argumento aqui pra fazer a analise procurando o begin e o end
            }
        }
}

int main (int argc, char* argv[]){
    //Requisito do professor que sejam sempre passsados 3 argumentos de entrada
    //que somados com o arquivo executado dao 4 argumentos no total.
    //Vamos fazer um teste se o segundo argumento eh um arquivo .asm, pois caso seja sabemos que devemos tratar o
    //programa como definido no trabalho 2 e caso nao seja devemos tratar como definido no trabalho 1
    if (((argc != 4) && (!string_ops::isAsmFile(argv[1]))) ||               //caso de erro do trabalho 1
        (((argc < 2) || (argc > 4)) && (string_ops::isAsmFile(argv[1])))){  //caso de erro do trabalho 2
         cout << "ERRO: Foi passado o numero errado de argumentos de entrada\n";
        return 0;
    }
    //o argv[1] contem o argumento de operacao (ex:-m), como so queremos a letra sem o '-', estou pegando o char do endereco deslocado de 1 byte
    //Para o trabalho 2 nao ha argumento de operacao, portanto caso o argv[1] seja um arquivo asm vamos usar o caractere de operacao 2 (para representar o trab 2)
    if(!string_ops::isAsmFile(argv[1])){
        trabalho_1(argc,argv);
    }
    else{
        trabalho_2(argc, argv);
        }
    return 0;
    }

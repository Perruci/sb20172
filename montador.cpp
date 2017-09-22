#include <iostream>

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

    switch (operation){
        //Operação de preprocessamento, coloca a estensão “.pre” no arquivo e somente 
        //avalia as diretivas EQU e IF.
        case 'p':
            cout << "Definir algo para fazer a operacao de preprocessamento\n";
            return 0;
            break;

        //Operação de processamento de macros, coloca a estensão “.mcr” no arquivo e somente avalia 
        //as diretivas EQU e IF e substitue as MACROS.
        case 'm':
            cout << "Definir algo para fazer a operacao de processamento\n";
            return 0;
            break;

        //Operação de motagem, coloca a estensão “.o” realiza a montagem de programa usando o PROCESSO DE PASSAGEM ÚNICA.
        case '-o':
            cout << "Definir algo para fazer a operacao de montagem\n";
            return 0;
            break;

        //So entrara no caso default se for uma operacao nao esperada, portanto significa que e erro
        default:
            cout << "ERRO: O argumento " << argv[1] << " passado, nao condiz com nenhum
            modo de operacao\n"
            return 0;

    }

}
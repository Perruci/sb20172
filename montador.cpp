#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <string.h>

using namespace std;

string pega_nome (char*);

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
    string arq_entrada = argv[2];
    
    //Pega o nome do arquivo de saida que sera criado pelo nosso codigo, no entanto pega o nome do arquivo sem a extensao, pois podemos criar ele com ate 3 extensoes diferentes no programa
    string arq_saida = pega_nome (argv[3]);

    switch (operation){
        //Operação de preprocessamento, coloca a estensão “.pre” no arquivo e somente 
        //avalia as diretivas EQU e IF.
        case 'p':
            //pre-procesasmento (arq_entrada, arq_saida);
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

    //Daqui pra baixo eu tinha feito para teste e para ver como ler o arquivo token por token, ainda quero tirar essa parte do main e colocar ela em cada uma das fucoes de pre-processamento,
    //processamento e montagem (ainda nao tive tempo pra terminar).

    //Pega o nome do arquivo de entrada passado por argumento da linha de comando e abre esse arquivo para leitura usando a 
    //classe ifstream e chamando o arquivo de code.
    ifstream code(argv[2]);
    
    //Cria o arquivo de saida que ira conter o resultado da montagem do programa com o nome indicado no ultimo argumento da linha de comando
    ofstream saida(argv[3]);

    //string de suporte para capturar os tokens
    string aux;

    //Loop para ler todas as linhas do codigo aberto e "tokenizar" elas (o token leva o : em consideracao ainda)
    while (getline(code, aux)){

        stringstream lineStream (aux);
        string token;

        while (lineStream >> token){
            saida << token << " ";
        }
    
        saida << endl;
    }

    //fecha o arquivo contendo o codigo assembly e o arquivo de saida
    code.close();
    saida.close();


    return 0;
}

/*****************************************************************************
Toda a declaracao da classe esta comentada pq tava dando um erro na compilacao e eu ainda nao tive tempo de ver  qual era o erro e arrumar
//Declaracao da classe Token geral para todos os Tokens do programa
class Token {
    public:
        string nome;  //field

        Token (string);  //construtor


}

//Declaracao do construtor da classe Token
Token::Token(string a){
    nome = a;
}

//Declarao dos metodos da classe Token


/*****************************************************************************/

//Funcao que analisa o nome do arquivo de saida passado como argumento de entrada e pega so a parte do nome antes da extensao (por exemplo: pega um triangulo.o e transforma em triangulo)
string pega_nome (char* nome){
    string aux = nome;
    string aux2;
    int tam = strlen (nome); //numero de caracteres da string nome

    for (int i = 0; i < tam; i++){
        if(nome[i] == '.'){
            break;
        }
        aux2 += nome[i];
    }

    return aux2;
}
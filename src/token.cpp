#include "../include/token.h"

//Declaracao do construtor da classe Token
Token::Token(std::string a){
    this -> nome = a;
    this -> type = token_type::undefined;
};

//Analisa se um determinado Token eh rotulo, tambem recebe a linha que esse token esta contido pra possiveis analises
bool Token::isRotulo(std::string token, std::string linha, std::vector<Mnemonic> instructionList){
    if (Token::have2points(token)){
        return true;
    }

    //algumas strings que queremos comparar, esta sendo feito assim pq de outras maneiras nao poderiamos ter um rotulo
    std::stringstream Line (linha);
    std::string aux;

    //pega o primeiro elemento da linha
    Line >> aux;

    //ve se a linha eh uma linha do tipo section
    if (aux == "section"){
        Line >> aux;
        if((aux == "data") || (aux == "text")){
            return false;
        }
    }

    //testa se eh uma instrucao
    for (int i = 0; i < instructionList.size(); i++){
        if (token == instructionList[i].nome){
            return false;
        }
    }

    //testa outros comandos que nao podem ser rotulos
    if ((token == "equ") || (token == "if") || (token == "macro") || (token == "end") || 
        (token == "space") || (token == "const") || (isInteger(token))){
        return false;
    }


    return true;
}

//Retorna 1 para rotulos de declaracao e 2 para chamadas de rotulo
//Se essa funcao for chamada, ja sabemos que se trata de um rotulo, portanto faz apenas o teste de ser declaracao
//se nao for,eh pq eh chamada
int Token::KindOfRotulo(std::string token){
    if (Token::have2points(token)){
        return 1;
    } else {
        return 2;
    }

}

//testa se o ultimo char da string eh ':', pois isso indica que ele eh uma declaracao de rotulo
bool Token::have2points(std::string token){
    char* ptr;
    ptr = &(token[0]);
    int tam = strlen(ptr);

    for(int i = 0; i < tam; i++){
        if ((token[i] == ':') &&(tam - i == 1)){
            return true;
        }
    }
    return false;
}


bool Token::isInteger(const std::string & aux)
{
   if(aux.empty() || ((!isdigit(aux[0])) && (aux[0] != '-') && (aux[0] != '+'))){
    return false;
   } 

   char* ptr;
   strtol(aux.c_str(), &ptr, 10);

   return (*ptr == 0) ;
}
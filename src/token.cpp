#include "../include/token.h"

//Declaracao do construtor da classe Token
Token::Token(std::string a){
    this -> nome = a;
    this -> type = token_type::undefined;
};

//Analisa se um determinado Token eh rotulo, tambem recebe a linha que esse token esta contido pra possiveis analises
//Retorna um inteiro que determina qual o "tipo" do rotulo
// 0-> nao eh rotulo
// 1-> eh declaracao de um rotulo
// 2-> eh chamada a um rotulo
// Ideia: talvez seja interessante a gente dividir as chamadas a rotulos entre chamadas a rotulos de diretivas ou de instrucoes
int Token::isRotulo(std::string token, std::string Line, std::vector<Mnemonic> instructionList){
    if (Token::have2points(token)){
        return 1;
    }

    return 0;
}

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

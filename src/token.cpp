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
bool Token::isRotulo(std::string token, std::string linha, std::vector<Mnemonic> instructionList){
    if (Token::have2points(token)){
        return true;
    }

    //Cria uma stringstream com a linha que ta sendo analisada
    std::stringstream line (linha);

    

    return false;
}

//Retorna 1 para rotulos de declaracao e 2 para chamadas de rotulo
int Token::KindOfRotulo(std::string token){

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

#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <iostream>
#include <string.h>
#include <vector>

#include "mnemonic.h"

// Tipos de token
// declarados fora da classe para que qualquer arquivo possa utilizá-los
namespace token_type
{
    // Cada uma das variáveis receve um inteiro constante em ordem crescente a partir de 0
    // Utilizaremos elas apenas para fins de comparação, portanto seus valores não são relevantes
    enum{
        menomonico,
        rotulo,
        macro,
        argumento,
        diretiva,
        undefined
    };
};

//Declaracao da classe Token geral para todos os Tokens do programa
class Token
{
    public:
        int type;
        std::string nome;
        Token(std::string);

        bool isRotulo (std::string, std::string, std::vector<Mnemonic>);
        bool have2points (std::string); //checa se tem : no final do token
};
#endif

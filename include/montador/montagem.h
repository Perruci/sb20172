#ifndef _MONTAGEM_H_
#define _MONTAGEM_H_

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <string.h>
#include <vector>
#include <tuple>

#include "token.h"
#include "mnemonic.h"
#include "rotulo.h"
#include "string_ops.h"
#include "macro.h"

#define TRABALHO_1 1
#define TRABALHO_2_ARQ_UNICO 2
#define TRABALHO_2_ARQS_MULT 3

class Montagem
{
private:
    std::string inputFileName;
    std::string outputFileName;
    std::ifstream fileText;
    std::ofstream fileOutput;

    std::vector<Mnemonic> instructionList;
    std::vector<Rotulo> rotulosList;
    std::vector<Token> tokensList;

    //lista de inteiros para preparar o arquivo objeto em memoria antes de imprimir no arquivo
    std::vector<int> outputFileList;
    //lista de inteiros para corrigir o número da linha do arquivo .mcr para o .asm
    std::vector<int> address_adjusts;;

    /***********************************************************************************************************************************************************/
    //Serie de variaveis booleanas para indicar do que a linha atual do programa se trata e facilitar a decisao do que deve ser feito
    //Declaradas na classe, pois serao utilizadas por varios metodos
    bool lineIsInstruction;             //caso ativo, significa que esperamos argumentos nos proximos tokens
    int numberOfOperandsInLine;         //vai ser um contador decrescente para checar se a quantidade de operandos requeridos por uma instrucao foi conferida
                                        //Da suporte a variavel lineIsInstruction

    bool lineIsSpace;                   //caso ativo, significa que podemos nao ter proximo token ou ter um numero
    int numberOfArgumentsInSpace;       //vai ser um contador decrescente para checar se mais de um argumento foi passado para o space
                                        //Da suporte a variavel lineIsSpace

    bool lineIsConst;                   //caso ativo, significa que esperamos um numero no proximo token
    int numberOfArgumentsInConst;       //vai ser um contador decrescente para checar se mais de um argumento foi passado para o const
                                        //Da suporte a variavel lineIsConst
    /***********************************************************************************************************************************************************/

    //Variaveis para tratar vetores
    bool argumentIsVector;
    int VectorValue = 1;

    int haveRotuloInLine = 0;            //controle para caso aparecam dois rotulos na mesma linha
    std::string lineRotuloName = "";     //salva o nome do rotulo daquela linha, caso exista um rotulo naquela linha

    int operationMode;                  //int que irá determinar o modo de operação da montagem


    //Fecha e abre o arquivo do codigo para atualizar o ponteiro de arquivo, caso alguma outra funcao tenha usado ele
    inline bool reopenCodeFile()
    {
        this->fileText.close();
        this->fileText.open(this->inputFileName);
        return(fileText.is_open());
    };

public:
    Montagem(std::string, std::string, std::vector<Mnemonic>&);
    ~Montagem();

    bool run(std::vector<int>, int);
    void printOutput();

    int getNumber (std::string);

    /* Rotulos */
    void trata_rotulos (std::string, int, int &, int);
    void declaracao_de_rotulo(std::string, int &, int);
    void chamada_de_rotulo(std::string, int &, int);
    void printRotulos();
    void trataRotulo_altoNivel(int, std::string, int &, int, bool, bool);
    void rotuloAtualizaEnds (int);
    void checkRotulos();

     /*Erros Lexicos*/
     void scannerLexico (std::string, char, int);
     void checkLexicalError(int);

     /*Instrucoes*/
     bool isInstruction(Token);
     int InstructionOperand(Token);
     int instructionOpcode(Token);
     void trata_instructions (int, bool, int);

     /*Diretivas*/
     bool isDiretiva(Token);
     void trata_diretivas(int, bool, int);
     void pegaValorDiretivas (std::string, int&);

     /*Section*/
     void trata_section(bool &, bool &, bool &, std::stringstream &, std::string &, int &);

     /*Correção de linhas */
     int getOriginalLine(int);

    std::string setOutputExtension(std::string);
    std::string setInputExtension(std::string);
};
#endif

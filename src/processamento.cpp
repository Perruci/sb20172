#include "../include/processamento.h"

Processamento::Processamento(std::string inputFile, std::string outputFile)
{
    this->inputFileName =  inputFile;
    this->outputFileName = outputFile;
    this->setInputExtension(".pre");
    this->setOutputExtension (".mcr");
    this->fileText.open(inputFileName);
    this->fileOutput.open(outputFileName);
}

Processamento::~Processamento()
{
    // Sem preocupações com o close
    fileText.close();
    fileOutput.close();
}

std::string Processamento::setOutputExtension(std::string extension)
{
    this->outputFileName = string_ops::setOutputExtension(inputFileName, extension);

    return outputFileName;
}

std::string Processamento::setInputExtension(std::string extension)
{
    this->inputFileName = string_ops::setOutputExtension(inputFileName, extension);

    return inputFileName;
}

//Metodo principal que vai realizar o processamento
bool Processamento::run(){
    std::string line;
    std::string word;

    //loop linha a linha para analisar o arquivo de entrada
    while (getline(this->fileText, line)){
        std::stringstream lineStream(line);

        //loop palavra a palavra para procurar as macros
        while (lineStream >> word){
            //caso ache uma macro, a palavra antes dela eh o seu rotulo
            if (word == "macro"){
                std::string rotulo = this-> getRotulo (line);

                //testa se houve erro na funcao que pega rotulo
                if (rotulo == "erro"){
                    return false;
                }

                //Caso nao houve erro pega o codigo relativo a macro
                std::string codigo = this ->getCode ();

                //testa se houve erro no metodo de pegar o codigo da macro
                if (codigo == "erro"){
                    return false;
                }

                //Se nao houve nenhum erro, cria a Macro
                Macro macro(rotulo, codigo);
                this->macrosList.push_back(macro);
            }
        }
    }
    this-> printMacros();
    return true;
}

//Metodo para pegar o rotulo da macro, considerando que o rotulo tem que ser o primeiro token da linha
std::string Processamento::getRotulo(std::string line){
    std::stringstream lineStream (line);
    std::string word;

    //Pega a primeira palavra da linha
    lineStream >> word;

    //cria um token com a primeira palavra da linha
    Token token(word);
    
    //caso os dois pontos estejam na string
    if (token.have2points(token.nome)){
        word = string_ops::trunca_nome(word, ':');
        return word;
    }

    //Caso chegue aqui pode ser que o usuario tenha digitado o rotulo, espaco e depois os :
    //Logo, devemos analisar se o segundo token da frase eh :
    lineStream >> word;
    if (word == ":"){
        //se isso ocorreu, entao o nome do Token ja eh o nome da declaracao da macro sem os :
        return token.nome;
    }

    //Se chegar aqui eh pq algum erro ocorreu
    std::cout << "Token invalido para declarar a macro\n";
    return "erro";   
}

//Metodo para pegar o codigo relativo a macro, utiliza-se do fileText que ja esta aberto
std::string Processamento::getCode (){
    std::string line;
    std::string word;
    std::string code;

    //como o arquivo ja esta aberto, a linha seguinte eh a linha logo abaixo da macro e sera o comeco do seu codigo
    while(getline(this->fileText,line)){
        std::stringstream lineStream(line);

        //pega token a token e vai salvando no code, ate achar end
        while (lineStream >> word){
            if (word == "end"){
                //terminou a macro
                return code;
            }
            //se nao for end, salva em code
            code += word;
            code += ' ';
        }
        //Salta a linha no code
        code += '\n';
    }

    //Se chegar aqui eh pq leu o arquivo todo e nao achou o fim da macro
    std::cout << "ERRO -> Macro inicializada, mas nao finalizada\n";
    return "erro";
}

//Metodo para debug
void Processamento::printMacros(){
    for (size_t i = 0; i < macrosList.size(); i++){
        std::cout << "macro name = " << macrosList[i].name << std::endl;
        std::cout << "macro code = " << macrosList[i].codigo << std::endl;
    }
}
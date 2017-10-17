#include "../include/montagem.h"

Montagem::Montagem(std::string inputFile, std::string outputFile, std::vector<Mnemonic>& instructionList)
{
    this->inputFileName =  inputFile;
    this->outputFileName = outputFile;
    this->setInputExtension(".mcr");
    this->setOutputExtension (".o");
    this->fileText.open(inputFileName);
    this->fileOutput.open(outputFileName);

    /* Pass as reference the instructionList */
    this->instructionList = instructionList;
}

Montagem::~Montagem()
{
    // Sem preocupações com o close
    fileText.close();
    fileOutput.close();
}

std::string Montagem::setOutputExtension(std::string extension)
{
    this->outputFileName = string_ops::setOutputExtension(inputFileName, extension);

    return outputFileName;
}

std::string Montagem::setInputExtension(std::string extension)
{
    this->inputFileName = string_ops::setOutputExtension(inputFileName, extension);

    return inputFileName;
}

//Metodo principal que vai realizar a montagem
bool Montagem::run(){
    bool check_section_text = false;  //passa a ser true quando acharmos a secao text
    int contador_tokens = 0;          //sera utilizado para navegar pela tokensList
    int contador_endereco = 0;        //sera utilizado para determinar o endereco de cada token do programa
    int haveRotuloInLine = 0;            //controle para caso aparecam dois rotulos na mesma linha
    int contador_de_linhas = 0;       //Controle para saber em qual linha do programa esta algum erro


    //Fecha e abre o arquivo do codigo para atualizar o ponteiro de arquivo, caso alguma outra funcao tenha usado ele
    this->reopenCodeFile();

    std::string line;
    std::string word;

    //Loop para fazer a montagem
    // para cada linha..
    while (getline(this->fileText, line))
    {
        // transforma para minuscula
        line = string_ops::minuscula(line);
        contador_de_linhas++;               //incrementa a linha

        std::stringstream lineStream (line);
        haveRotuloInLine = 0;               //prepara a variavel para  analisar o proximo rotulo

        //Loop para analisar cada termo um a um
        while (lineStream >> word)
        {
            //coloca o termo na lista
            this->tokensList.push_back(Token(word));

            //se o token for um rotulo, trata ele
            if (tokensList[contador_tokens].isRotulo(word, line, instructionList)){
                int tipo = tokensList[contador_tokens].KindOfRotulo(word);

                //testa se eh uma declaracao de rotulo e se ja tem outra declaracao na mesma linha
                if (tipo == tipo_rotulo::declaracao)
                {
                    //se ja tiver uma declaracao na linha sera erro
                    if (haveRotuloInLine == 1)
                    {
                        std::cout << "Mais de um Rotulo na linha " << contador_de_linhas << "\n";
                        return false;
                    } else
                    {
                        haveRotuloInLine = 1;
                        this->trata_rotulos(word, tipo, contador_endereco);
                    }
                }
                if (tipo == tipo_rotulo::chamada)
                {
                    this->trata_rotulos(word, tipo, contador_endereco);
                }
            }
            contador_tokens++;
        }
    }
    this->printRotulos();
    return true;
}

void Montagem::trata_rotulos (std::string token, int tipo_rotulo, int endereco){
    //Retira, caso tenha, o :
    token = string_ops::trunca_nome(token, ':');

    //se for a declaracao de um rotulo, chama a rotina pra tratar isso
    if(tipo_rotulo == tipo_rotulo::declaracao)
    {
        declaracao_de_rotulo(token, endereco);
        return;
    }

    //se for chamada de um rotulo, chama a rotina pra tratar isso
    if (tipo_rotulo == tipo_rotulo::chamada)
    {
        chamada_de_rotulo(token, endereco);
        return;
    }

    return;
}

void Montagem::declaracao_de_rotulo(std::string token, int endereco)
{
    for (size_t i = 0; i < rotulosList.size(); i++)
    {
        //se ele ja existir na lista, devemos apenas atualizar o endereco de declaracao dele e o estado
        if (token == rotulosList[i].name)
        {
            rotulosList[i].setState(endereco);
            //ADD UMA ROTINA PARA TRATAR ONDE O ROTULO JA FOI ENCONTRADO ANTES, OU SEJA,
            //VOLTAR NO ARQUIVO E ATUALIZAR OS ENDERECOS. PRECISO NA MONTAGEM
            return;
        }
    }
    //se ele ainda nao existir na lista, criamos ele agora
    Rotulo rotulo (token, true, endereco);
    rotulosList.push_back(rotulo);
    return;
}

void Montagem::chamada_de_rotulo(std::string token, int endereco)
{
    for (size_t i = 0; i < rotulosList.size(); i++)
    {
        //se ele ja existir na lista, devemos colocar no lugar o endereco dele caso ja esteja declarado, ou adicionar
        //esse endereco na lista de lugares onde ele eh chamado para tratar depois
        if (token == rotulosList[i].name)
        {
            if(rotulosList[i].alreadyDeclared)
            {
                //ADD UMA ROTINA PARA TRATAR A PARTE DA MONTAGEM
                return;
            } else
            {
                rotulosList[i].addressList.push_back(endereco);
                return;
            }
        }
    }
    //Caso ainda nao exista um rotulo com esse nome
    // Cria um novo rótulo e adiciona-o a lista
    Rotulo rotulo (token, false, 0);
    rotulo.addressList.push_back(endereco);
    rotulosList.push_back(rotulo);
    return;
}

void Montagem::printRotulos(){
    for (size_t i = 0; i < this->rotulosList.size(); i++){
        std::cout << this->rotulosList[i].name << ' ';
        for(size_t j = 0; j < this->rotulosList[i].addressList.size(); j++){
            std::cout << this->rotulosList[i].addressList[j] << ' ';
        }
        std::cout << std::endl;
    }
}


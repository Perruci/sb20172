#include "../include/pre_processamento.h"

Pre_Processamento::Pre_Processamento(std::string inputFile, std::string outputFile, std::vector<Mnemonic>& instructionList)
{
    this->inputFileName =  inputFile;
    this->outputFileName = outputFile;
    this->setOutputExtension(".pre");
    this->fileText.open(inputFileName);
    this->fileOutput.open(outputFileName);
    /* Pass as reference the instructionList */
    this->instructionList = instructionList;
}

Pre_Processamento::~Pre_Processamento()
{
    // Sem preocupações com o close
    fileText.close();
    fileOutput.close();
}

std::string Pre_Processamento::setOutputExtension(std::string extension)
{
    this->outputFileName = string_ops::setOutputExtension(outputFileName, extension);

    return outputFileName;
}

//Faz a etapa de pre-processamento e trata os EQU e IF, funcao criada sem utilizar a tokenizacao, pois estou com receio de que
//se fizermos a tokenizacao nessa parte estariamos violando a regra de passagem unica
bool Pre_Processamento::run(){
    bool check_section_text = false;  //passa a ser true quando acharmos a secao text
    int contador_tokens = 0;          //sera utilizado para navegar pela tokensList
    int contador_endereco = 0;        //sera utilizado para determinar o endereco de cada token do programa
    int haveRotuloInLine = 0;            //controle para caso aparecam dois rotulos na mesma linha
    int contador_de_linhas = 0;       //Controle para saber em qual linha do programa esta algum erro

    //Fecha e abre o arquivo do codigo para atualizar o ponteiro de arquivo, caso alguma outra funcao tenha usado ele
    this->reopenCodeFile();

    std::string line;
    std::string word;

    //Loop para fazer o pre-processamento
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
            // se encontra um comentário, pula a linha
            if (word[0] == ';')
                break;

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

            if (word == "text")
            {
                check_section_text = true;
            }

            if ((word == "equ"))
            {

            }

           /* if (word == "if"){
                lineStream >> word; //pega o rotulo que vem depois do if
                for (int i = 0; i < rotulos.size(); i++){
                    //Se for verdade pega a linha debaixo e imprime ela no arquivo de saida
                    if ((word == rotulos[i]) && (true_or_false[i] != '0')){
                        getline(this->fileText, aux);
                        ptr = &(aux[0]);
                        aux = Pre_Processamento::minuscula(ptr);
                        std::stringstream lineStream_aux2(aux);

                        while (lineStream_aux2 >> word){
                            out_pre << word << " ";
                        }
                    }
                    //Se o if representar uma mentira (rotulo = 0), nao imprime a linha debaixo no arquivo
                    if ((word == rotulos[i]) && (true_or_false[i] == '0')){
                        getline (this->fileText, aux);
                        std::stringstream lineStream_aux2(aux);

                        while (lineStream_aux2 >> word){
                            //do nothing
                        }
                    }
                }
            }*/
        }
    }
    //Ta aqui so pra debugar
    this->printRotulos();
}

/* Tratamento de Rótulos */

//checa a lista de rotulos procurando se determinado rotulo ja foi visto antes, retorno com o seguinte significado
// 0-> primeira vez que encontramos o rotulo
// 1xx-> rotulo ja encontrado e ja declarado (xx eh o indice da lista de rotulos onde esse rotulo se encontra)
// 2xx-> rotulo ja encontrado, mas ainda nao declarado (xx vide de cima)
int Pre_Processamento::RotuloAlreadyFound(std::string token){
    //percorre a lista de rotulos procurando pelo rotulo
    for (size_t i = 0; i < rotulosList.size() ; i++){
        //se encontrar o rotulo
        if (rotulosList[i].name == token){
            if (rotulosList[i].alreadyDeclared){
                return (100 + i);
            } else {
                return (200 + i);
            }
        }
    }
    return 0;
}

void Pre_Processamento::trata_rotulos (std::string token, int tipo_rotulo, int endereco){
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

void Pre_Processamento::declaracao_de_rotulo(std::string token, int endereco)
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

void Pre_Processamento::chamada_de_rotulo(std::string token, int endereco)
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

void Pre_Processamento::printRotulos(){
    for (size_t i = 0; i < rotulosList.size(); i++){
        std::cout << rotulosList[i].name << ' ';
        for(size_t j = 0; j < rotulosList[i].addressList.size(); j++){
            std::cout << rotulosList[i].addressList[j] << ' ';
        }
        std::cout << std::endl;
    }
}

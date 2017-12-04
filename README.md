# Software Básico 2017/2
# Trabalho 1

## Autores:
* Pedro Perruci (14/0158596)
* Tiago Gallo (14/0164138)

## Compilação
A compilação do projeto foi simplificada a uma chamada do comando:
``` bash
make
```
Os comandos do arquivo Makefile são, então, executados e compilam o programa main.cpp
referenciando suas dependências na pasta include/ e src/.
Resultarão 3 arquivos executaveis da chamada:
    * montador
    * ligador
    * carregador

## Execução
### Montador
A execução do programa está de acordo com as especificações do trabalho.
Deve-se realizar a chamada do montador da seguinte maneira:
``` bash
./montador path/to/asm_file1.asm path/to/asm_file2.asm ... path/to/asm_fileN.asm
```
Sendo asm_file os arquivos em assembly inventado que deseja-se montar.
Foram testados até 2 arquivos de entrada, logo 1 <= N <= 3.
São gerados N arquivos de saída, de extensão '.o', nos mesmos diretórios dos arquivos de entrada.
### Ligador
Deve-se executar o programa, chamando os arquivos objeto gerados pelo montador.
Espera-se a seguinte chamada:
``` bash
./ligador path/to/obj_file1.o path/to/obj_file2.o ... path/to/obj_fileN.o
```
Gera-se, como saída um arquivo ligado no mesmo diretório da chamada do ligador.
Este arquivo não possui extensão.
### Carregador
A chamada do carregador realiza, tanto a simulação do programa, como gera um arquivo com a imagem de memória.
Espera-se receber dados com relação à memória do sistema no momento da chamada do programa:
``` bash
./carregador linked_file [NUM_CHUNKS] [CHUNK_SIZES] [CHUNK_ADDRESSES]
```
Lembre-se de que linked_file deve estar no diretório em que foi chamado o programa ligador.
O parâmetro [NUM_CHUNKS] define o número de chunks de memória disponíveis para montar o executável.
Seja N o número de chunks passado, deve-se informar N tamanhos de chunks e N endereços de chunk.
Caso existam comandos input no código, o terminal aguardará a entrada do usuário para continuar a execução.
A instrução output imprime no terminal.

## Limitações Conhecidas
### Montador
A chamada de endereços do tipo r+1 não é reconhecida pelo montador.
Espera-se a chamada do endereço espaçada entre seus caracteres.
Ou seja: r + 1.
### Carregador
A imagem de memória gerada possui a correção de indices relativos ao chunk atual.
Dependências entre chunks de memória não têm seu endereço ajustado conforme chunks posteriores.
Por exemplo, para 2 chunks de tamanho 2 e 1 começando nos endereços 0 e 20, respectivamente:
chunk 1, size: 2
0 12    (input)
1 3     [VAR]
2 14    (stop)
chunk 2, size: 1
20 0    (Var)

Vale notar, também, que a imagem de memória é impressa antes da execução do programa, conforme esclarecido com o professor em sala.

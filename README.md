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
O nome do executável resultante será: montador

## Execução
A execução do programa está de acordo com as especificações do trabalho.
Deve-se realizar a chamada do executável da seguinte maneira:
``` bash
./montador -[flag] path/to/asm_file path/to/outputfile
```
Sendo as possíveis flags:
* -p: realiza apenas a fase de pre-processamento do montador;
* -m: realiza o pre-processamento e tratamento das macros;
* -o: realiza o processo de montagem completo.

Serão gerados arquivos de saída com extensões '.pre', '.mcr' e '.o'
com as saídas do pre-processamento, processamento de macros e montagem, respectivamente.

Vale notar que foram apenas testados arquivos de entrada e saída em um mesmo diretório.

## Estrutura da Entrega:
``` bash
.
├── LICENSE
├── Makefile
├── README.md
├── include
│   ├── macro.h
│   ├── mnemonic.h
│   ├── montador.h
│   ├── montagem.h
│   ├── pre_processamento.h
│   ├── processamento.h
│   ├── rotulo.h
│   ├── string_ops.h
│   └── token.h
├── main.cpp
├── montador
├── src
│   ├── macro.cpp
│   ├── mnemonic.cpp
│   ├── montador.cpp
│   ├── montagem.cpp
│   ├── pre_processamento.cpp
│   ├── processamento.cpp
│   ├── rotulo.cpp
│   └── token.cpp
└── tables
    └── instructions.txt
```

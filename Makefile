TARGET_MONTADOR=montador
TARGET_CARREGADOR=carregador
CC=g++ -std=c++11 -Wall
CFLAGS=-I
HEADERS=include/
SRC=src/carregador/*.cpp src/ligador/*.cpp src/montador/*.cpp src/mem_sim/*.cpp

default: all

main:
	clear
	$(CC) -o $(TARGET_MONTADOR) main.cpp $(SRC) $(CFLAGS) $(HEADERS)

simulador:
	clear
	$(CC) -o $(TARGET_CARREGADOR) carregador.cpp $(SRC) $(CFLAGS) $(HEADERS)

all:
	clear
	$(CC) -o $(TARGET_MONTADOR) main.cpp $(SRC) $(CFLAGS) $(HEADERS)
	$(CC) -o $(TARGET_CARREGADOR) carregador.cpp $(SRC) $(CFLAGS) $(HEADERS)

clean:
	rm -f montador carregador

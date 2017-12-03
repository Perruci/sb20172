TARGET_M=montador
TARGET_L=ligador
TARGET_C=carregador
CC=g++ -std=c++11 -Wall
CFLAGS=-I
HEADERS=include/
SRC=src/carregador/*.cpp src/ligador/*.cpp src/montador/*.cpp src/mem_sim/*.cpp src/exec_sim/*.cpp

default: all

main:
	clear
	$(CC) -o $(TARGET_M) main.cpp $(SRC) $(CFLAGS) $(HEADERS)

ligador:
	clear
	$(CC) -o $(TARGET_L) main_ligador.cpp $(SRC) $(CFLAGS) $(HEADERS)

carregador:
	clear
	$(CC) -o $(TARGET_C) main_carregador.cpp $(SRC) $(CFLAGS) $(HEADERS)

all:
	clear
	$(CC) -o $(TARGET_M) main.cpp $(SRC) $(CFLAGS) $(HEADERS)
	$(CC) -o $(TARGET_L) main_ligador.cpp $(SRC) $(CFLAGS) $(HEADERS)
	$(CC) -o $(TARGET_C) main_carregador.cpp $(SRC) $(CFLAGS) $(HEADERS)

clean:
	rm -f montador ligador carregador

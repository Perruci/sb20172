TARGET_M=montador
TARGET_L=ligador
CC=g++ -std=c++11 -Wall
CFLAGS=-I
HEADERS=include/
SRC=src/carregador/*.cpp src/ligador/*.cpp src/montador/*.cpp

default: main

main:
	clear
	$(CC) -o $(TARGET_M) main.cpp $(SRC) $(CFLAGS) $(HEADERS)

ligador:
	clear
	$(CC) -o $(TARGET_L) main_ligador.cpp $(SRC) $(CFLAGS) $(HEADERS)

clean:
	rm -f montador ligador

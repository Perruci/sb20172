TARGET=montador
CC=g++
CFLAGS=-I
HEADERS=include/
SRC=src/*.cpp

default: main

main:
	$(CC) -o $(TARGET) montador.cpp $(SRC) $(CFLAGS) $(HEADERS)

clean:
	rm -f montador

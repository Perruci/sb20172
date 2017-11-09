TARGET=montador
CC=g++ -std=c++11 -Wall
CFLAGS=-I
HEADERS=include/
SRC=src/montador/*.cpp

default: main

main:
	clear
	$(CC) -o $(TARGET) main.cpp $(SRC) $(CFLAGS) $(HEADERS)

clean:
	rm -f montador

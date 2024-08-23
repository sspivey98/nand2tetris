CC=g++
CFLAGS=-std=c++11 -Wall

.PHONY: all clean debug

all: CFLAGS+=-D NDEBUG -O3
all: vm	

vm: main.o
	$(CC) $(CFLAGS) Parser.o CodeWriter.o main.o -o vm

Parser.o: Parser.cpp
	$(CC) $(CFLAGS) -c Parser.cpp

CodeWriter.o: CodeWriter.cpp Parser.o
	$(CC) $(CFLAGS) -c CodeWriter.cpp

main.o: main.cpp Parser.o CodeWriter.o
	$(CC) $(CFLAGS) -c main.cpp

clean:
	rm -rf *.o vm

debug: CFLAGS+=-g
debug: vm	

BIN=bin
SRC=src
INC=inc

all: main.o hello.o
	g++ ${BIN}/main.o ${BIN}/hello.o -o ${BIN}/hello

main.o: ${SRC}/main.cpp
	g++ -c ${SRC}/main.cpp -o ${BIN}/main.o -I ${INC} 

hello.o: ${SRC}/hello.cpp
	g++ -c ${SRC}/hello.cpp -o ${BIN}/hello.o -I ${INC}

clean:
	rm -rf ${BIN}/*.o ${BIN}/hello

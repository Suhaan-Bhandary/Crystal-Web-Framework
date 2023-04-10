CC=g++
CFLAGS=-c -Wall

# This Target will compile all the file
# run is dependency
all: server

server: build/main.o build/Logger.o build/TcpServer.o
	${CC} build/main.o build/Logger.o build/TcpServer.o -o server

build/main.o: src/main.cpp
	${CC} ${CFLAGS} src/main.cpp -o build/main.o

build/Logger.o: src/Logger/Logger.cpp
	${CC} ${CFLAGS} src/Logger/Logger.cpp -o build/Logger.o

build/TcpServer.o: src/TcpServer/TcpServer.cpp
	${CC} ${CFLAGS} src/TcpServer/TcpServer.cpp -o build/TcpServer.o

clean:
	rm -rf build/*o server
CC=g++
CFLAGS=-c
# CFLAGS=-c -Wall

# This Target will compile all the file
# run is dependency
all: server

server: build/main.o build/TcpServer.o build/Router.o build/Request.o build/Response.o build/Controller.o build/Utils.o build/Logger.o
	${CC} build/main.o build/TcpServer.o build/Router.o build/Request.o build/Response.o build/Controller.o build/Utils.o build/Logger.o -o server

build/main.o: src/main.cpp
	${CC} ${CFLAGS} src/main.cpp -o build/main.o

build/TcpServer.o: src/TcpServer/TcpServer.cpp
	${CC} ${CFLAGS} src/TcpServer/TcpServer.cpp -o build/TcpServer.o

build/Router.o: src/Router/Router.cpp
	${CC} ${CFLAGS} src/Router/Router.cpp -o build/Router.o

build/Request.o: src/Request/Request.cpp
	${CC} ${CFLAGS} src/Request/Request.cpp -o build/Request.o

build/Response.o: src/Response/Response.cpp
	${CC} ${CFLAGS} src/Response/Response.cpp -o build/Response.o

build/Controller.o: src/Controller/Controller.cpp
	${CC} ${CFLAGS} src/Controller/Controller.cpp -o build/Controller.o

build/Utils.o: src/Utils/Utils.cpp
	${CC} ${CFLAGS} src/Utils/Utils.cpp -o build/Utils.o

build/Logger.o: src/Logger/Logger.cpp
	${CC} ${CFLAGS} src/Logger/Logger.cpp -o build/Logger.o

clean:
	rm -rf build/*o server
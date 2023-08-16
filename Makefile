CC=g++
CFLAGS=-c -DDEVELOPMENT_ENVIRONMENT
# CFLAGS=-c -Wall

# This Target will compile all the file
# run is dependency
all: httpScratchServer

httpScratchServer: build/main.o build/TcpServer.o build/Router.o build/UserRouter.o build/Request.o build/Response.o build/HTMLTemplate.o build/StatusCodes.o build/systemController.o build/mainController.o build/Utils.o build/Logger.o build/Json.o build/JsonNode.o 
	${CC} build/main.o build/TcpServer.o build/Router.o build/UserRouter.o build/Request.o build/Response.o build/HTMLTemplate.o build/StatusCodes.o build/systemController.o build/mainController.o build/Utils.o build/Logger.o build/Json.o build/JsonNode.o  -o httpScratchServer

build/main.o: src/main.cpp
	${CC} ${CFLAGS} src/main.cpp -o build/main.o

build/TcpServer.o: src/TcpServer/TcpServer.cpp
	${CC} ${CFLAGS} src/TcpServer/TcpServer.cpp -o build/TcpServer.o

build/Router.o: src/Router/Router.cpp
	${CC} ${CFLAGS} src/Router/Router.cpp -o build/Router.o

build/UserRouter.o: app/Router/UserRouter.cpp
	${CC} ${CFLAGS} app/Router/UserRouter.cpp -o build/UserRouter.o

build/Request.o: src/Request/Request.cpp
	${CC} ${CFLAGS} src/Request/Request.cpp -o build/Request.o

build/Response.o: src/Response/Response.cpp
	${CC} ${CFLAGS} src/Response/Response.cpp -o build/Response.o

build/HTMLTemplate.o: src/HTMLTemplate/HTMLTemplate.cpp
	${CC} ${CFLAGS} src/HTMLTemplate/HTMLTemplate.cpp -o build/HTMLTemplate.o

build/StatusCodes.o: src/Response/StatusCodes.cpp
	${CC} ${CFLAGS} src/Response/StatusCodes.cpp -o build/StatusCodes.o

build/systemController.o: src/Controller/systemController.cpp
	${CC} ${CFLAGS} src/Controller/systemController.cpp -o build/systemController.o

build/mainController.o: app/Controller/mainController.cpp
	${CC} ${CFLAGS} app/Controller/mainController.cpp -o build/mainController.o

build/Utils.o: src/Utils/Utils.cpp
	${CC} ${CFLAGS} src/Utils/Utils.cpp -o build/Utils.o

build/Logger.o: src/Logger/Logger.cpp
	${CC} ${CFLAGS} src/Logger/Logger.cpp -o build/Logger.o

build/Json.o: src/Json/Json.cpp
	${CC} ${CFLAGS} src/Json/Json.cpp -o build/Json.o

build/JsonNode.o: src/Json/JsonNode.cpp
	${CC} ${CFLAGS} src/Json/JsonNode.cpp -o build/JsonNode.o

# Cleaning the build
clean:
	rm -rf build/*o httpScratchServer

# Testing code
testProject: compileTests
	./testProject && rm ./testProject

compileTests: build/tests.o build/JsonTests.o build/TrimTests.o build/HTMLTemplateTests.o build/Logger.o build/Utils.o build/Json.o build/JsonNode.o build/HTMLTemplate.o
	${CC} build/tests.o build/JsonTests.o build/TrimTests.o build/HTMLTemplateTests.o build/Logger.o build/Utils.o build/Json.o build/JsonNode.o build/HTMLTemplate.o -o testProject

build/tests.o: tests/tests.cpp
	${CC} ${CFLAGS} tests/tests.cpp -o build/tests.o

build/JsonTests.o: tests/JsonTests/JsonTests.cpp
	${CC} ${CFLAGS} tests/JsonTests/JsonTests.cpp -o build/JsonTests.o

build/TrimTests.o: tests/TrimTests/TrimTests.cpp
	${CC} ${CFLAGS} tests/TrimTests/TrimTests.cpp -o build/TrimTests.o

build/HTMLTemplateTests.o: tests/HTMLTemplateTests/HTMLTemplateTests.cpp
	${CC} ${CFLAGS} tests/HTMLTemplateTests/HTMLTemplateTests.cpp -o build/HTMLTemplateTests.o

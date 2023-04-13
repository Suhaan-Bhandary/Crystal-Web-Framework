#include "TcpServer/TcpServer.h"

int main() {
    // Creating TCP/IP Server
    http::TcpServer server = http::TcpServer("127.0.0.1", 8080);
    server.startListen();

    return 0;
}

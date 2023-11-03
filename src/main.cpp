#include "../app/db/models/initializeTables.h"
#include "SQLite/SQLite.h"
#include "TcpServer/TcpServer.h"

int main() {
    // Configure Database
    database::SQLite::configDatabase("server.db");

    // Load models
    database::initializeTables();

    // Creating TCP/IP Server
    http::TcpServer server = http::TcpServer("127.0.0.1", 8080);
    server.startListen();

    return 0;
}

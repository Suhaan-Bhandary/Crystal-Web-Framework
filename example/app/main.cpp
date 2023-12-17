#include "./Router/UserRouter.h"
#include "./db/models/initializeTables.h"
#include "crystal.h"

int main() {
    // Set public and template folder path relative to the root folder
    Crystal::Config::TEMPLATE_DIR_PATH = "/example/app/templates/";
    Crystal::Config::PUBLIC_DIR_PATH = "/example/app/public/";

    // Configure Database
    Crystal::Database::SQLite::configDatabase("server.db");

    // Load models
    database::initializeTables();

    // Creating TCP/IP Server
    Crystal::TcpServer server = Crystal::TcpServer("127.0.0.1", 8080);
    userRouterPaths(server.router);

    // Start the server
    server.startListen();

    return 0;
}

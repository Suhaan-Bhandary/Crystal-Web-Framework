#include "SQLite.h"

#include <sqlite3.h>

#include "../Logger/Logger.h"

namespace database {
std::string SQLite::dbPath = "";

SQLite::SQLite() {
    LOGGER("Connecting to Database...");
    int rc = sqlite3_open(dbPath.c_str(), &db);
    if (rc) {
        LOGGER_ERROR("Failed to connect to database");
        exit(1);
    }
}

SQLite::~SQLite() {
    LOGGER("Closing Database Connection...");
    sqlite3_close(db);
}

void SQLite::configDatabase(std::string path) { dbPath = path; }

SQLite& SQLite::getInstance() {
    if (dbPath.empty()) {
        LOGGER_ERROR("Database Path not Specified");
        exit(1);
    }

    // This instance will be created once
    static SQLite instance;
    return instance;
}

// To give access to private member db
sqlite3* SQLite::getConnection() { return db; }

}  // namespace database

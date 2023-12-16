#include "initializeTables.h"
#include "crystal.h"

void createVisitorsTable(sqlite3 *db) {
    const char *query = "CREATE TABLE IF NOT EXISTS Visitors (timestamp TEXT);";

    char *errMsg = 0;
    int rc = sqlite3_exec(db, query, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        LOGGER_ERROR("Error in creating Visitors table:", errMsg);
        sqlite3_free(errMsg);
    } else {
        LOGGER("Visitors Table created successfully.");
    }
}

void database::initializeTables() {
    database::SQLite &databaseInstance = database::SQLite::getInstance();
    sqlite3 *db = databaseInstance.getConnection();

    createVisitorsTable(db);
}
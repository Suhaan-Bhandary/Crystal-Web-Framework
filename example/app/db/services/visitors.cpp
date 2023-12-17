#include "visitors.h"

#include "crystal.h"

namespace database::services {
int getVisitorsCount() {
    auto& databaseInstance = Crystal::Database::SQLite::getInstance();
    sqlite3* db = databaseInstance.getConnection();

    int count = -1;
    const char* countQuery = "SELECT COUNT(*) FROM visitors;";

    char* errMsg;
    int rc = sqlite3_exec(
        db, countQuery,
        [](void* data, int argc, char** argv, char** azColName) -> int {
            int* count = reinterpret_cast<int*>(data);
            *count = std::stoi(argv[0]);
            return 0;
        },
        &count, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "Error executing query: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }

    return count;
}
void addVisitor() {
    auto& databaseInstance = Crystal::Database::SQLite::getInstance();
    sqlite3* db = databaseInstance.getConnection();

    const std::string insertSQL =
        "INSERT INTO visitors (timestamp) VALUES (?);";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, insertSQL.c_str(), -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        LOGGER_ERROR("Error preparing statement:", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // Time stamp
    std::string timestamp = "dummy time";

    // Bind the parameters
    sqlite3_bind_text(stmt, 2, timestamp.c_str(), -1, SQLITE_STATIC);

    // Execute the statement
    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        LOGGER_ERROR("Error inserting data:", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
}
}  // namespace database::services

// The ifndef is used so that the same File is not loaded again
#ifndef INCLUDED_SQLITE
#define INCLUDED_SQLITE

#include <sqlite3.h>
#include <string.h>

#include "../Logger/Logger.h"

namespace Crystal::Database {
class SQLite {
   private:
    sqlite3* db;

    SQLite();
    ~SQLite();

   public:
    static std::string dbPath;
    static void configDatabase(std::string path);
    static SQLite& getInstance();
    sqlite3* getConnection();
};
}  // namespace Crystal::Database

#endif
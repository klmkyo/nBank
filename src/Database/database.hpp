#include <sqlite_orm/sqlite_orm.h>
#include "structures.hpp"

#define DB_PATH "db.sqlite"

#ifndef _DATABASE_HEADER
#define _DATABASE_HEADER

/* Storage type built in StructureBuilder */
typedef decltype(StructureBuilder::_build_structure()) Storage;

/*
Database service class - CRUD usage example:
Database::getStorage()->insert(...);

[!] It should be synced before any write or read operation (once per application runtime)
Database::getStorage()->sync_schema();
*/
class Database
{
    public:
    /// @return Shared pointer of Storage object (SQLite_ORM) 
    static std::shared_ptr<Storage> getStorage() {
        return std::make_shared<Storage>(StructureBuilder::_build_structure(DB_PATH));
    };
};

#endif
#include <sqlite_orm/sqlite_orm.h>
#include "structures.hpp"

#define DB_PATH "db.sqlite"

#ifndef _DATABASE_HEADER
#define _DATABASE_HEADER

/*
Builds a new sqlite storage object and links it to specified structures
*/
auto _build_structure(const char* path = "")
{
    using namespace sqlite_orm;
    return make_storage(path,
                                make_table<User>("users",
                                        make_column("id", &User::id, autoincrement(), primary_key()),
                                        make_column("name", &User::name)),
                                make_table<Account>("accounts",
                                        make_column("id", &Account::id, autoincrement(), primary_key()),
                                        make_column("user_id", &Account::user_id),
                                        make_column("name", &Account::name),
                                        make_column("balance", &Account::balance)),
                                make_table<CreditCard>("credit_cards",
                                        make_column("id", &CreditCard::id, autoincrement(), primary_key()),
                                        make_column("account_id", &CreditCard::account_id),
                                        make_column("name", &CreditCard::name),
                                        make_column("number", &CreditCard::number),
                                        make_column("cvv", &CreditCard::cvv),
                                        make_column("expiration_month", &CreditCard::expiration_month),
                                        make_column("expiration_year", &CreditCard::expiration_year),
                                        make_column("pin", &CreditCard::pin)));
}

typedef decltype(_build_structure()) Storage;

/*
Database service class - CRUD usage example:
Database::getStorage()->insert(...);

[!] It should be synced before any write or read operation (once per application runtime)
Database::getStorage()->sync_schema();
*/
class Database
{
    public:
    static std::shared_ptr<Storage> getStorage() {
        return std::make_shared<Storage>(_build_structure(DB_PATH));
    };
};

#endif
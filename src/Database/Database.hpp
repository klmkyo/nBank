#pragma once
#include <sqlite_orm/sqlite_orm.h>
#include "Database/User.hpp"
#include "Database/Account.hpp"
#include "Database/CreditCard.hpp"
#include "Database/TransactionData.hpp"


#define DB_PATH "db.sqlite"

/* Structure Builder - should be used only once (in Database definition)*/
class StructureBuilder {
  public:
  /*
    Builds a new sqlite storage object and links it to specified structures:
    'users' table -> User
    'accounts' table -> Account
    'credit_cards' table -> CreditCard
    'transactions' table -> TransactionData
  */
  static auto _build_structure(const char* path = "")
    {
        using namespace sqlite_orm;
        return make_storage(path,
                make_table<User>("users",
                        make_column("id", &User::id, autoincrement(), primary_key()),
                        make_column("login", &User::login, unique()),
                        make_column("password_hash", &User::password_hash),
                        make_column("password_salt", &User::password_salt),
                        make_column("name", &User::name)),
                make_table<Account>("accounts",
                        make_column("id", &Account::id, autoincrement(), primary_key()),
                        make_column("user_id", &Account::user_id),
                        make_column("name", &Account::name),
                        make_column("balance", &Account::balance),
                        make_column("phone_number", &Account::phone_number)),
                make_table<CreditCard>("credit_cards",
                        make_column("id", &CreditCard::id, autoincrement(), primary_key()),
                        make_column("account_id", &CreditCard::account_id),
                        make_column("name", &CreditCard::name),
                        make_column("number", &CreditCard::number),
                        make_column("cvv", &CreditCard::cvv),
                        make_column("expiration_month", &CreditCard::expiration_month),
                        make_column("expiration_year", &CreditCard::expiration_year),
                        make_column("pin", &CreditCard::pin)),
                make_table<TransactionData>("transactions",
                        make_column("id", &TransactionData::id, autoincrement(), primary_key()),
                        make_column("sender_account_id", &TransactionData::sender_account_id),
                        make_column("recipient_account_id", &TransactionData::recipient_account_id),
                        make_column("amount", &TransactionData::amount)));
    };
};

/* Storage type built in StructureBuilder */
typedef decltype(StructureBuilder::_build_structure()) Storage;

/*
Database service class - CRUD usage example:
Database::getStorage()->insert(...);

[!] It should be synced before any write or read operation (once per application runtime)
Database::getStorage()->sync_schema();
*/

class Database {
    public:
    /// @return Shared pointer of Storage object (SQLite_ORM) 
    static std::shared_ptr<Storage> getStorage() {
        return std::make_shared<Storage>(StructureBuilder::_build_structure(DB_PATH));
    };
};


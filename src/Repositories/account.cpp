#include "account.hpp"
#include "Repositories/repos.hpp"
#include <sqlite_orm/sqlite_orm.h>

/// @brief Search for account by id
/// @param id
/// @return unique_ptr<Account> if found, otherwise nullptr
std::unique_ptr<Account> AccountRepo::GetAccountById(uint32_t id)
{
    if (auto acc = Database::getStorage()->get_pointer<Account>(id)){
        return acc;
    } else {
        return nullptr;
    }
}

/// @brief Insert new account into the database, use overwrite if you want to replace existing account
/// @param account 
/// @param overwrite (optional, default: false)
/// @return ID of created account (if failed, returns -1)
uint32_t AccountRepo::InsertAccount(const Account& account, bool overwrite)
{
    auto _account = AccountRepo::GetAccountById(account.id);
    uint32_t id = -1;
    if (_account && overwrite){
        try{
            Database::getStorage()->replace(account);
            id = account.id;
        }catch(...) {
            // TODO: Exception handling
            return -1;
        }
    } else {
        try{
            id = Database::getStorage()->insert(account);
        }catch(...) {
            // TODO: Exception handling
            return -1;
        }
    }
    return id;
}

bool AccountRepo::UpdateAccount(const Account& account)
{
    if (auto _account = AccountRepo::GetAccountById(account.id)){
        try{
            Database::getStorage()->update(account);
        } catch(...){
            return false;
        }
        return true;
    } else {
        return false; // account not found
    }
}
#pragma once
#include "../Database/database.hpp"

class AccountRepo
{
    public:
    std::unique_ptr<Account> GetAccountById(uint32_t id);
    uint32_t InsertAccount(const Account& account, bool overwrite = false);
    bool UpdateAccount(const Account& account);
};


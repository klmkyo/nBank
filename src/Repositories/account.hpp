#pragma once
#include "../Database/database.hpp"

class AccountRepo
{
    public:
    std::unique_ptr<Account> GetAccountById(uint32_t id);
    uint32_t InsertAccount(const Account& account, bool overwrite = false);
    bool UpdateAccount(const Account& account);
};

enum CreateCreditCardResult {
    SUCCESS,
    FIELDS_EMPTY,
    INTERNAL_ERROR
};

CreateCreditCardResult CreateCreditCard(int account_id, std::string name, int pin);
std::vector<CreditCard> GetCreditCardsByAccountId(int account_id);
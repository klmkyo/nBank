#pragma once
#include "../Database/database.hpp"

class CreditCardRepo
{
    public:
    std::unique_ptr<CreditCard> GetCreditCardById(uint32_t id);
    uint32_t InsertCreditCard(const CreditCard& account, bool overwrite = false);
    bool UpdateCreditCard(const CreditCard& account);
};


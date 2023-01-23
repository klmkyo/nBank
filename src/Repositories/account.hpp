#pragma once
#include "Database/database.hpp"

enum CreateCreditCardResult {
    SUCCESS,
    FIELDS_EMPTY,
    INTERNAL_ERROR
};

CreateCreditCardResult CreateCreditCard(int account_id, std::string name, int pin);
std::vector<CreditCard> GetCreditCardsByAccountId(int account_id);
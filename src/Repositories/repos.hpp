#include "user.hpp"
#include "account.hpp"
#include "creditcard.hpp"
#pragma once

class Repo
{
    public:
    /// @return User repository pointer
    static std::shared_ptr<UserRepo> User() {
        return std::make_shared<UserRepo>();
    }
    /// @return Account repository pointer
    static std::shared_ptr<AccountRepo> Account() {
        return std::make_shared<AccountRepo>();
    }
    /// @return CreditCard repository pointer
    static std::shared_ptr<CreditCardRepo> CreditCard() {
        return std::make_shared<CreditCardRepo>();
    }
};
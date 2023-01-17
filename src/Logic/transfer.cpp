#include "transfer.hpp"

// "amount" parameter should be always positive in Transfer constructor
Transfer::Transfer(Account* account, double amount, Account* recipient) 
    : Transaction(account, -amount)
{
    if (amount < 0) 
        this->amount = -amount;
    this->recipient = recipient;
}

bool Transfer::Execute(TransactionResult& result) {
    if (this->recipient == nullptr){
        result = TransactionResult::ACCOUNT_NOT_FOUND;
        return false;
    }
    // checks and executes internal transaction first, only then transfers the money
    if (Transaction::Execute(result))
    {
        // amount is stored negative
        this->recipient->balance += -(this->amount);
        Repo::Account()->UpdateAccount(*(this->recipient));
        return true;
    }
    return false;
}


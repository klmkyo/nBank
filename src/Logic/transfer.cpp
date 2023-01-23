#include "Logic/transfer.hpp"

// "amount" parameter should be always positive in Transfer constructor
Transfer::Transfer(Account* account, double amount, Account* recipient) 
    : Transaction(account, -amount)
{
    if (amount < 0) 
        this->amount = -amount;
    this->recipient = recipient;
}

bool Transfer::Execute(TransactionResult& result) {
    if (this->recipient == nullptr) {
        result = TransactionResult::ACCOUNT_NOT_FOUND;
        resultString = ResultToString(result);
        LogAction(false);
        return false;
    }
    
    // checks and executes internal transaction first, only then transfers the money
    if (Transaction::Execute(result)) {
        // amount is stored negative
        this->recipient->balance += -(this->amount);
        Repo<Account>::Update(*(this->recipient));
        TransactionData t {*account, *recipient, -amount};
        Repo<TransactionData>::Insert(t);
        return true;
    }
    return false;
}


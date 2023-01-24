#include "Logic/transfer.hpp"
#include "Database/Repo.hpp"

// "amount" parameter should be always positive in Transfer constructor
Transfer::Transfer(Account& account, double amount) 
    : Transaction(account, -amount)
{
    this->hasRecipient = false;
    if (amount < 0) 
        this->amount = -amount;
}

void Transfer::SetRecipient(Account& recipient){
    this->recipient = recipient;
    this->hasRecipient = true;
}

bool Transfer::Execute(TransactionResult& result) {
    if (!hasRecipient) {
        result = TransactionResult::ACCOUNT_NOT_FOUND;
        resultString = ResultToString(result);
        LogAction(false);
        return false;
    }
    
    // checks and executes internal transaction first, only then transfers the money
    if (Transaction::Execute(result)) {
        // amount is stored negative
        recipient.balance += -(amount);
        Repo<Account>::Update(recipient);
        TransactionData t {account, recipient, -amount};
        Repo<TransactionData>::Insert(t);
        return true;
    }
    return false;
}


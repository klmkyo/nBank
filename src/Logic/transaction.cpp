#include "Logic/transaction.hpp"



Transaction::Transaction(Account& account, double amount){
    this->account = account;
    this->hasAccount = true;
    this->amount = amount;
}

void Transaction::SetAccount(Account& account){
    this->account = account;
    this->hasAccount = true;
}

std::string Transaction::ResultToString(const TransactionResult& tr) {
    switch (tr){
        case TransactionResult::ACCOUNT_NOT_FOUND:
        return "account not found";
        
        case TransactionResult::WRONG_PIN:
        return "wrong pin";

        case TransactionResult::NO_MONEY:
        return "not enough money";

        default:
        return "success";
    }
}

bool Transaction::CheckExecute(TransactionResult& result){
    if (!hasAccount){
        result = TransactionResult::ACCOUNT_NOT_FOUND;
        return false;
    }
    if (requirePin && wrongPin){
        result = TransactionResult::WRONG_PIN;
        return false;
    }
    if (amount < 0 && account.balance < amount){
        result = TransactionResult::NO_MONEY;
        return false;
    }
    result = TransactionResult::SUCCESS;
    return true;
}

bool Transaction::Execute(TransactionResult& result){
    if (this->CheckExecute(result)){
        account.balance += amount;
        Repo<Account>::Update(this->account);
    }
    bool ret = result == TransactionResult::SUCCESS;
    resultString = ResultToString(result);
    LogAction(ret);
    return ret;
}
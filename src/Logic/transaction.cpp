#include "Logic/transaction.hpp"



Transaction::Transaction(Account* account, double amount){
    this->account = account;
    this->amount = amount;
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
    if (this->account == nullptr){
        result = TransactionResult::ACCOUNT_NOT_FOUND;
        return false;
    }
    if (this->requirePin && this->wrongPin){
        result = TransactionResult::WRONG_PIN;
        return false;
    }
    if (this->amount < 0 && this->account->balance < this->amount){
        result = TransactionResult::NO_MONEY;
        return false;
    }
    result = TransactionResult::SUCCESS;
    return true;
}

bool Transaction::Execute(TransactionResult& result){
    if (this->CheckExecute(result)){
        this->account->balance += this->amount;
        Repo<Account>::Update(*(this->account));
        //Database::getStorage()->update(*account);
    }
    bool ret = result == TransactionResult::SUCCESS;
    resultString = ResultToString(result);
    LogAction(ret);
    return ret;
}
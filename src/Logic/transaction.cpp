#include "transaction.hpp"

Transaction::Transaction(Account* account, double amount){
    this->account = account;
    this->amount = amount;
}

bool Transaction::CheckExecute(TransactionResult& result){
    if (this->account == nullptr){
        result = TransactionResult::ACCOUNT_NOT_FOUND;
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
        Repo::Account()->UpdateAccount(*(this->account));
        // TODO: log transaction ?
        return true;
    }
    return false;
}
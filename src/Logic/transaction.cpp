#include "transaction.hpp"

Transaction::Transaction(Account* account, double amount){
    this->account = account;
    this->amount = amount;
}

bool Transaction::CheckExecute(){
    return this->RequirePIN() 
        && this->amount >= account->balance;
}

bool Transaction::Execute(){
    if (this->CheckExecute()){
        account->balance -= this->amount;
        // log transaction
        return true;
    }
    return false;
}
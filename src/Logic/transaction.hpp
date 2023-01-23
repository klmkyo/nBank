#pragma once
#include "Repositories/user.hpp"
#include "Repositories/repos.hpp"

enum class TransactionResult {
    ACCOUNT_NOT_FOUND,
    WRONG_PIN,
    NO_MONEY,
    SUCCESS
};

class Transaction {

protected:
    double amount = 0.0;
    Account* account = nullptr;
    bool requirePin = false;
    bool wrongPin = true;    

public:
    Transaction(){};
    Transaction(double amount){this->amount = amount;};
    Transaction(Account* account, double amount);
    virtual ~Transaction() {};

    virtual bool CheckExecute(TransactionResult& result);
    virtual bool Execute(TransactionResult& result);
};


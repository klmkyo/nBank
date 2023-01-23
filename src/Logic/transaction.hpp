#pragma once
#include "Repositories/user.hpp"
#include "Repositories/repos.hpp"
#include "logger.hpp"

enum class TransactionResult {
    ACCOUNT_NOT_FOUND,
    WRONG_PIN,
    NO_MONEY,
    SUCCESS
};

class Transaction : LoggedOperation {

protected:
    double amount = 0.0;
    Account* account = nullptr;
    bool requirePin = false;
    bool wrongPin = true;
    std::string resultString;

public:
    Transaction(){};
    Transaction(double amount){this->amount = amount;};
    Transaction(Account* account, double amount);
    virtual ~Transaction() {};
    void LogAction(bool success) {
        LoggedOperation::LogAction(success);
    }

    std::string ResultToString(const TransactionResult& tr);
    virtual bool CheckExecute(TransactionResult& result);
    virtual bool Execute(TransactionResult& result);
};


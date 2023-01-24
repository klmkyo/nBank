#pragma once
#include "Database/Account.hpp"
#include "Database/User.hpp"
#include "Logger.hpp"

enum class TransactionResult {
    ACCOUNT_NOT_FOUND,
    WRONG_PIN,
    NO_MONEY,
    SUCCESS
};


/// @brief Default use case of Transaction class is removing specified amount from the account.
/// fe: with Transaction(&acc, 15) constructor call, 'Execute' call will remove 15 PLN from the account
class Transaction : LoggedOperation {

protected:
    double amount = 0.0;
    Account account;
    bool hasAccount = false;
    bool requirePin = false;
    bool wrongPin = true;
    std::string resultString;

public:
    Transaction(){};
    Transaction(double amount){this->amount = amount;};
    Transaction(Account& account, double amount);
    virtual ~Transaction() {};
    void SetAccount(Account& account);
    void LogAction(bool success) {
        LoggedOperation::LogAction(success);
    }

    std::string ResultToString(const TransactionResult& tr);
    virtual bool CheckExecute(TransactionResult& result);
    virtual bool Execute(TransactionResult& result);
};


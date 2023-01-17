#include <Repositories/user.hpp>
#include <Repositories/repos.hpp>

#ifndef _TRANSACTION_HEADER
#define _TRANSACTION_HEADER

enum class TransactionResult {
    ACCOUNT_NOT_FOUND,
    WRONG_PIN,
    NO_MONEY,
    SUCCESS
};

class Transaction {
    private:
    Account* account = nullptr;
    

    protected:
    double amount = 0.0;
    

    public:
    Transaction(Account* account, double amount);

    virtual bool CheckExecute(TransactionResult& result);

    virtual bool Execute(TransactionResult& result);

};

#endif
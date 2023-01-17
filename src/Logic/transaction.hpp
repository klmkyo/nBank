#include <Repositories/user.hpp>

#ifndef _TRANSACTION_HEADER
#define _TRANSACTION_HEADER

class Transaction {
    private:
    Account* account = nullptr;
    double amount = 0.0;

    protected:
    virtual bool RequirePIN(){return true;};

    public:
    Transaction(Account* account, double amount);

    virtual bool CheckExecute();

    virtual bool Execute();

};

#endif
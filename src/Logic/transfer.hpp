#include "transaction.hpp"

#ifndef _TRANSFER_HEADER
#define _TRANSFER_HEADER

class Transfer : public Transaction {
    protected:
    Account* recipient;

    public:
    Transfer(Account* account, double amount, Account* recipient = nullptr);
    
    virtual bool Execute(TransactionResult& result);
};

#endif
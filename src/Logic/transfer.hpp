#include "transaction.hpp"

#ifndef _TRANSFER_HEADER
#define _TRANSFER_HEADER

class Transfer : public Transaction {
    private:
    Account* recipient;

    public:
    Transfer(Account* account, double amount, Account* recipient);
    
    virtual bool Execute(TransactionResult& result);
};

#endif
#pragma once
#include "Logic/transaction.hpp"

class Transfer : public Transaction {
protected:
    Account* recipient;

public:
    Transfer(Account* account, double amount, Account* recipient = nullptr);
    virtual bool Execute(TransactionResult& result);

};


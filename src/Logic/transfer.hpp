#pragma once
#include "Logic/transaction.hpp"

/// @brief Transfer class is a type of Transaction, where the money goes from
/// one account to another. [!] Amount used in constructor should be always positive.
/// fe. Transfer(&acc1, 15, &acc2) will transfer 15 PLN from acc1 to acc2
class Transfer : public Transaction {
protected:
    Account* recipient;

public:
    Transfer(Account* account, double amount, Account* recipient = nullptr);
    virtual bool Execute(TransactionResult& result);

};


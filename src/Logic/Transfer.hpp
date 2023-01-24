#pragma once
#include "Logic/Transaction.hpp"

/// @brief Transfer class is a type of Transaction, where the money goes from
/// one account to another. [!] Amount used in constructor should be always positive.
/// fe. Transfer(&acc1, 15, &acc2) will transfer 15 PLN from acc1 to acc2
class Transfer : public Transaction {
protected:
    Account recipient;
    bool hasRecipient;

public:
    Transfer(Account& account, double amount);
    virtual bool Execute(TransactionResult& result) override;
    void SetRecipient(Account& recipient);

};


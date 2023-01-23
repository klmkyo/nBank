#pragma once
#include "Logic/transfer.hpp"

class TransactionHistory {
public:
    TransactionHistory() {}
    void logTransaction(Transfer& t);
};
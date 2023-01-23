#pragma once
#include <string>
#include <iostream>
#include "Database/Account.hpp"

class TransactionData {
public:
  uint32_t id;

  uint32_t sender_account_id;
  uint32_t recipient_account_id;
  double amount;

  TransactionData(uint32_t sender_account_id, uint32_t recipient_account_id, double amount);
  TransactionData(const Account& sender, const Account& recipent, double amount);

  // empty constructor for sqlite_orm
  TransactionData() = default;
};

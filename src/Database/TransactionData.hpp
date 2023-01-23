#pragma once
#include <string>
#include <iostream>
#include "Database/Account.hpp"

class TransactionData {
public:
  uint32_t id;

  uint32_t sender_id;
  uint32_t recipent_id;
  double amount;

  TransactionData(uint32_t sender_id, uint32_t recipent_id, double amount);
  TransactionData(const Account& sender, const Account& recipent, double amount);
};

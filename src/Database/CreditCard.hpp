#pragma once
#include <string>
#include <iostream>

class CreditCard {
public:
  uint32_t id;

  uint32_t account_id;
  std::string name;

  int number;
  int cvv;
  int expiration_month;
  int expiration_year;
  int pin;

  friend std::ostream& operator<<(std::ostream& os, const CreditCard& cc) {
    return os << "CreditCard(id=" << cc.id << ", name=" << cc.name << ", account_id="<<cc.account_id
    <<", number="<<cc.number<<", cvv="<<cc.cvv<<", expiration_month="<<cc.expiration_month<<", expiration_year="
    <<cc.expiration_year<<", pin="<<cc.pin<<")";
  }
};

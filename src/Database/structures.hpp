#pragma once
#include <iostream>
#include <set>
#include <string>


struct User {
  int id;

  std::string name;
  std::string login;
  std::string password_hash;
  std::string password_salt;

  friend std::ostream& operator<<(std::ostream& os, const User& u) {
    return os << "User(id=" << u.id << ", name=" << u.name << " login=" << u.login << " password_hash=" << u.password_hash << " password_salt=" << u.password_salt << ")";
  }
};



struct Account {
  public:
  int id;

  int user_id;
  std::string name;
  double balance;
  int phone_number;


  Account() = default;
  Account(uint32_t user_id, std::string name, double balance ,int phone_number): 
                user_id(user_id), name(name), balance(balance), phone_number(phone_number) {}

  friend std::ostream& operator<<(std::ostream& os, const Account& acc) {
    return os << "Account(id=" << acc.id << ", name=" << acc.name << ", user="<<acc.user_id<<", balance="<<acc.balance<<", phone_number="<<acc.phone_number<<")";
  }
};

struct CreditCard {
  int id;

  int account_id;
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


struct TransactionData {
  uint32_t id;

  uint32_t sender_id;
  uint32_t recipent_id;
  double amount;

  TransactionData(uint32_t sender_id, uint32_t recipent_id, double amount): 
                  sender_id(sender_id), recipent_id(recipent_id), amount(amount) {}
  TransactionData(const Account& sender, const Account& recipent, double amount) : amount(amount) {
    sender_id = sender.user_id;
    recipent_id = recipent.user_id;
  } 
                  
};



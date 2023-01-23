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
  int id;

  int user_id;
  std::string name;
  double balance;
  int phone_number;

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

/* Structure Builder - should be used only once (in Database definition)*/
class StructureBuilder{
  public:
  /*
    Builds a new sqlite storage object and links it to specified structures:
    'users' table -> User
    'accounts' table -> Account
    'credit_cards' table -> CreditCard
    'transactions' table -> TransactionData
  */
  static auto _build_structure(const char* path = "")
    {
        using namespace sqlite_orm;
        return make_storage(path,
                make_table<User>("users",
                        make_column("id", &User::id, autoincrement(), primary_key()),
                        make_column("login", &User::login, unique()),
                        make_column("password_hash", &User::password_hash),
                        make_column("password_salt", &User::password_salt),
                        make_column("name", &User::name)),
                make_table<Account>("accounts",
                        make_column("id", &Account::id, autoincrement(), primary_key()),
                        make_column("user_id", &Account::user_id),
                        make_column("name", &Account::name),
                        make_column("balance", &Account::balance),
                        make_column("phone_number", &Account::phone_number)),
                make_table<CreditCard>("credit_cards",
                        make_column("id", &CreditCard::id, autoincrement(), primary_key()),
                        make_column("account_id", &CreditCard::account_id),
                        make_column("name", &CreditCard::name),
                        make_column("number", &CreditCard::number),
                        make_column("cvv", &CreditCard::cvv),
                        make_column("expiration_month", &CreditCard::expiration_month),
                        make_column("expiration_year", &CreditCard::expiration_year),
                        make_column("pin", &CreditCard::pin)),
                make_table<TransactionData>("transactions",
                        make_column("id", &TransactionData::id, autoincrement(), primary_key()),
                        make_column("sender_id", &TransactionData::sender_id),
                        make_column("recipent_id", &TransactionData::recipent_id),
                        make_column("amount", &TransactionData::amount)));
    };
};


#include <iostream>
#include <set>

struct User {
  int id;

  std::string name;
};

struct Account {
  int id;

  int user_id;
  std::string name;
  double balance;
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
};
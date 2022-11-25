#include <iostream>
#include <set>

#ifndef _STRUCTURES_HEADER
#define _STRUCTURES_HEADER

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

/* Structure Builder - should be used only once (in Database definition)*/
class StructureBuilder{
  public:
  /*
    Builds a new sqlite storage object and links it to specified structures:
    'users' table -> User
    'accounts' table -> Account
    'credit_cards' table -> CreditCard
  */
  static auto _build_structure(const char* path = "")
    {
        using namespace sqlite_orm;
        return make_storage(path,
                make_table<User>("users",
                        make_column("id", &User::id, autoincrement(), primary_key()),
                        make_column("name", &User::name)),
                make_table<Account>("accounts",
                        make_column("id", &Account::id, autoincrement(), primary_key()),
                        make_column("user_id", &Account::user_id),
                        make_column("name", &Account::name),
                        make_column("balance", &Account::balance)),
                make_table<CreditCard>("credit_cards",
                        make_column("id", &CreditCard::id, autoincrement(), primary_key()),
                        make_column("account_id", &CreditCard::account_id),
                        make_column("name", &CreditCard::name),
                        make_column("number", &CreditCard::number),
                        make_column("cvv", &CreditCard::cvv),
                        make_column("expiration_month", &CreditCard::expiration_month),
                        make_column("expiration_year", &CreditCard::expiration_year),
                        make_column("pin", &CreditCard::pin)));
    };
};

#endif
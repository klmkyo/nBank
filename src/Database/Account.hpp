#pragma once
#include <string>
#include <iostream>
#include <sqlite_orm/sqlite_orm.h>


enum class AccountValuesCheckStatus {
    SUCCESS,
    FIELDS_EMPTY,
    PHONE_NUMBER_EXISTS
};

class Account {
public:
    int id;

    int user_id;
    std::string name;
    double balance;
    int phone_number;


    Account() = default; //need for orm
    Account(uint32_t user_id, std::string name, double balance, int phone_number);
    static AccountValuesCheckStatus checkAccountValues(uint32_t user_id, std::string name, int phone_number);

    friend std::ostream& operator<<(std::ostream& os, const Account& acc) {
    return os << "Account(id=" << acc.id << ", name=" << acc.name << ", user="<<acc.user_id<<", balance="<<acc.balance<<", phone_number="<<acc.phone_number<<")";
    }
};
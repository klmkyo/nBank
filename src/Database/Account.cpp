#include "Database/Account.hpp"
#include "Database/Database.hpp"


Account::Account(uint32_t user_id, std::string name, double balance, int phone_number): 
                user_id(user_id), name(name), balance(balance), phone_number(phone_number) {
}

ParsingAccountValuesStatus Account::parseAccountValues(uint32_t user_id, std::string name, int phone_number) {
    using namespace sqlite_orm;
    // sprawdź czy pola nie są puste
    if (name.empty() || phone_number < 0){
        return ParsingAccountValuesStatus::FIELDS_EMPTY;
    }

    // sprawdź czy numer telefonu jest unikalny
    auto accounts = Database::getStorage()->get_all<Account>(where(c(&Account::phone_number) == phone_number));

    if (accounts.size() > 0) {
        return ParsingAccountValuesStatus::PHONE_NUMBER_EXISTS;
    }
    return ParsingAccountValuesStatus::SUCCESS;
}
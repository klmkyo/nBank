#include <sqlite_orm/sqlite_orm.h>
#include "Utils/Utils.hpp"
#include "Repositories/repos.hpp"
#include "UI/transferui.hpp"

/// @brief Create new credit card for user
/// @param account_id
/// @param pin
CreateCreditCardResult CreateCreditCard(int account_id, std::string name, int pin)
{
    using namespace sqlite_orm;

    if (name.empty()){
        return FIELDS_EMPTY;
    }

    // check if pin is 4 digits
    if (pin < 1000 || pin > 9999){
        return FIELDS_EMPTY;
    }

    // get account
    //auto account = Repo::Account()->GetAccountById(account_id);
    auto account = Repo<Account>::GetById(account_id);

    if (account){
        CreditCard card;
        card.pin = pin;
        card.account_id = account_id;
        card.name = name;

        // cvv is a 3 digit number
        card.cvv = std::stoi(Utils::gen_random_num(3));

        // expiration date is 5 years from now
        auto five_years = std::chrono::system_clock::now() + std::chrono::hours(24*365*5);

        // extract year and month from time_point
        auto time_t = std::chrono::system_clock::to_time_t(five_years);
        auto tm = *std::localtime(&time_t);
        card.expiration_year = tm.tm_year + 1900;
        card.expiration_month = tm.tm_mon + 1;

        // randomly generate card number (unique), and cvv
        int number;
        while (true) {
            number = std::stoi(Utils::gen_random_num(8));
            auto results = Database::getStorage()->get_all<CreditCard>(where(c(&CreditCard::number) == card.number));
            if (results.empty()){
                break;
            }
        }
        card.number = number;

        try {
            Repo<CreditCard>::Insert(card);
        } catch(...) {
            return INTERNAL_ERROR;
        }
        return SUCCESS;
    } else {
        return INTERNAL_ERROR;
    }
}

std::vector<CreditCard> GetCreditCardsByAccountId(int account_id){
    using namespace sqlite_orm;

    auto cards = Database::getStorage()->get_all<CreditCard>(where(c(&CreditCard::account_id) == account_id));
    return cards;
}

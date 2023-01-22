#include "Repositories/repos.hpp"
#include <sqlite_orm/sqlite_orm.h>
#include "UI/transferui.hpp"
#include "account.hpp"
#include "creditcard.hpp"

/// @brief Search for account by id
/// @param id
/// @return unique_ptr<Account> if found, otherwise nullptr
std::unique_ptr<Account> AccountRepo::GetAccountById(uint32_t id)
{
    if (auto acc = Database::getStorage()->get_pointer<Account>(id)){
        return acc;
    } else {
        return nullptr;
    }
}

/// @brief Insert new account into the database, use overwrite if you want to replace existing account
/// @param account 
/// @param overwrite (optional, default: false)
/// @return ID of created account (if failed, returns -1)
uint32_t AccountRepo::InsertAccount(const Account& account, bool overwrite)
{
    auto _account = AccountRepo::GetAccountById(account.id);
    uint32_t id = -1;
    if (_account && overwrite){
        try{
            Database::getStorage()->replace(account);
            id = account.id;
        }catch(...) {
            // TODO: Exception handling
            return -1;
        }
    } else {
        try{
            id = Database::getStorage()->insert(account);
        }catch(...) {
            // TODO: Exception handling
            return -1;
        }
    }
    return id;
}

bool AccountRepo::UpdateAccount(const Account& account)
{
    if (auto _account = AccountRepo::GetAccountById(account.id)){
        try{
            Database::getStorage()->update(account);
        } catch(...){
            return false;
        }
        return true;
    } else {
        return false; // account not found
    }
}

std::string gen_random_num(const int len) {
    std::string tmp_s;
    static const char alphanum[] =
        "0123456789";

    srand((unsigned) time(NULL));

    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i)
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];

    return tmp_s;
}

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
    auto account = Repo::Account()->GetAccountById(account_id);

    if (account){
        CreditCard card;
        card.pin = pin;
        card.account_id = account_id;
        card.name = name;

        // cvv is a 3 digit number
        card.cvv = std::stoi(gen_random_num(3));

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
            number = std::stoi(gen_random_num(8));
            auto results = Database::getStorage()->get_all<CreditCard>(where(c(&CreditCard::number) == card.number));
            if (results.empty()){
                break;
            }
        }
        card.number = number;

        try{
            Repo::CreditCard()->InsertCreditCard(card);
        } catch(...){
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

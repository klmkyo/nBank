#pragma once
#include <sqlite_orm/sqlite_orm.h>
#include <chrono>
#include "Database/Database.hpp"
#include "Database/User.hpp"
#include "Utils/Utils.hpp"
#include "Database/Repo.hpp"


// enums

enum class LoginResult {
    SUCCESS,
    WRONG_PASSWORD,
    USER_NOT_FOUND,
    INTERNAL_ERROR
};

struct LoginResponse {
    LoginResult status;
    User user;
};

enum class RegisterResult {
    SUCCESS,
    USER_EXISTS,
    FIELDS_EMPTY,
    INTERNAL_ERROR
};

enum CreateCreditCardResult {
    SUCCESS,
    FIELDS_EMPTY,
    INTERNAL_ERROR
};

std::vector<Account> GetUserAccounts(uint32_t uid) {
    using namespace sqlite_orm;
    auto accounts = Database::getStorage()->get_all<Account>(where(c(&Account::user_id) == uid));
    return accounts;
}

std::vector<CreditCard> GetCreditCardsByAccountId(int account_id) {
    using namespace sqlite_orm;
    auto cards = Database::getStorage()->get_all<CreditCard>(where(c(&CreditCard::account_id) == account_id));
    return cards;
}

std::vector<TransactionData> GetTransactionsByAccountId(int account_id) {
    using namespace sqlite_orm;
    // recepient id or sender id has to be equal to account_id
    auto transactions = Database::getStorage()->get_all<TransactionData>(where(c(&TransactionData::recipient_account_id) == account_id || c(&TransactionData::sender_account_id) == account_id));
    return transactions;
}


std::vector<std::string> getAccountNames(const std::vector<Account>& accounts) {
    std::vector<std::string> accountNames;
    for (auto& account : accounts) {
        accountNames.push_back(account.name);
    }
    return accountNames;
}

std::vector<std::string> getCreditCardNames(const std::vector<CreditCard>& credit_cards) {
    std::vector<std::string> creditCardNames;
    for (auto& credit_card : credit_cards) {
        creditCardNames.push_back(credit_card.name);
    }
    return creditCardNames;
}

LoginResponse Login(const std::string& login, const std::string& password) {
    using namespace sqlite_orm;
    // znajdź użytkownika
    // sqlite_orm nie wspiera znajdywnia po unique, poniższy kod nie zadziała
    // auto user = Database::getStorage()->get_optional<User>(where(c(&User::login) == login));
    // dlatego musimy użyć get_all
    auto users = Database::getStorage()->get_all<User>(where(c(&User::login) == login));
    if (users.size() == 0){
        return LoginResponse { LoginResult::USER_NOT_FOUND };
    } else if (users.size() > 1){
        // to się nie powinno zdarzyć, ale w razie czego zwróć błąd
        std::cerr << "Znaleziono kilku użytkowników o takim samym loginie!" << std::endl;
        exit(1);
    } else {
        // sprawdź hasło
        if (Utils::verify_password(password, users[0].password_hash, users[0].password_salt)){
            return LoginResponse { LoginResult::SUCCESS, users[0] };
        } else {
            return LoginResponse { LoginResult::WRONG_PASSWORD };
        }
    }
}

RegisterResult Register(const std::string& login, const std::string& name, const std::string& password) {
    using namespace sqlite_orm;
    // sprawdź czy pola nie są puste
    if (login.empty() || password.empty() || name.empty()){
        return RegisterResult::FIELDS_EMPTY;
    }

    // sprawdź czy użytkownik o takim loginie już istnieje
    auto users = Database::getStorage()->get_all<User>(where(c(&User::login) == login));
    if (users.size() > 0){
        return RegisterResult::USER_EXISTS;
    } else {
        // zarejestruj użytkownika
        User user;
        user.login = login;
        user.name = name;
        auto hp = Utils::HashPassword(password);
        user.password_hash = hp.hash;
        user.password_salt = hp.salt;
        if (Repo<User>::Insert(user) == -1){
            return RegisterResult::INTERNAL_ERROR;
        } else {
            return RegisterResult::SUCCESS;
        }
    }
}



/// @brief Create new credit card for user
/// @param account_id
/// @param pin
CreateCreditCardResult CreateCreditCard(int account_id, std::string name, int pin) {
    using namespace sqlite_orm;

    if (name.empty()){
        return FIELDS_EMPTY;
    }

    // check if pin is 4 digits
    if (pin < 1000 || pin > 9999){
        return FIELDS_EMPTY;
    }

    // get account
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


#include "Repositories/repos.hpp"
#include "Repositories/user.hpp"


LoginResponse Login(const std::string& login, const std::string& password)
{
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

RegisterResult Register(const std::string& login, const std::string& name, const std::string& password)
{
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

std::vector<Account> GetUserAccounts(uint32_t uid)
{
    using namespace sqlite_orm;
    auto accounts = Database::getStorage()->get_all<Account>(where(c(&Account::user_id) == uid));
    return accounts;
}

// tworzy rachunek dla użytkownika
CreateAccountResult CreateUserAccount(const uint32_t user_id, const std::string& name, int phone_number)
{
    using namespace sqlite_orm;
    // sprawdź czy pola nie są puste
    if (name.empty() || phone_number < 0){
        return CreateAccountResult::FIELDS_EMPTY;
    }

    Account acc {user_id, name, 0, phone_number};

    // sprawdź czy numer telefonu jest unikalny
    auto accounts = Database::getStorage()->get_all<Account>(where(c(&Account::phone_number) == phone_number));
    if (accounts.size() > 0){
        return CreateAccountResult::PHONE_NUMBER_EXISTS;
    }

    auto id = Database::getStorage()->insert(acc);
    if (id == -1){
        return CreateAccountResult::INTERNAL_ERROR;
    } else {
        return CreateAccountResult::SUCCESS;
    }
}



#include "../Utils/sha256.hpp"
#include <Repositories/repos.hpp>

/// @brief Search for user by id
/// @param uid 
/// @return unique_ptr<User> if found, otherwise nullptr
std::unique_ptr<User> UserRepo::GetUserById(uint32_t uid)
{
    if (auto u = Database::getStorage()->get_pointer<User>(uid)){
        return u;
    } else {
        return nullptr;
    }
}

/// @brief Insert new user into the database, use overwrite if you want to replace existing user
/// @param user 
/// @param overwrite (optional, default: false)
/// @return ID of created user (if failed, returns -1)
uint32_t UserRepo::InsertUser(const User& user, bool overwrite)
{
    auto _user = UserRepo::GetUserById(user.id);
    uint32_t id = -1;
    if (_user && overwrite){
        try{
            Database::getStorage()->replace(user);
            id = user.id;
        }catch(...) {
            // TODO: Exception handling
            return -1;
        }
    } else {
        try{
            id = Database::getStorage()->insert(user);
        }catch(...) {
            // TODO: Exception handling
            return -1;
        }
    }
    return id;
}

/// @brief Hash password using sha2
/// @param password
/// @return HashedPassword
HashedPassword HashPassword(const std::string& password)
{
    srand(time(NULL));
    HashedPassword hp;
    // random salt
    hp.salt = gen_random(16);
    // hash password
    hp.hash = simple_sha256_hash(password + hp.salt);
    return hp;
}

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
        if (VerifyPassword(password, users[0].password_hash, users[0].password_salt)){
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
        auto hp = HashPassword(password);
        user.password_hash = hp.hash;
        user.password_salt = hp.salt;
        if (Repo::User()->InsertUser(user) == -1){
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
CreateAccountResult CreateUserAccount(const uint32_t user_id, const std::string& name, double balance, int phone_number)
{
    using namespace sqlite_orm;
    // sprawdź czy pola nie są puste
    if (name.empty() || balance < 0 || phone_number < 0){
        return CreateAccountResult::FIELDS_EMPTY;
    }

    Account acc;
    acc.name = name;
    acc.balance = balance;
    acc.phone_number = phone_number;
    acc.user_id = user_id;

    auto id = Database::getStorage()->insert(acc);
    if (id == -1){
        return CreateAccountResult::INTERNAL_ERROR;
    } else {
        return CreateAccountResult::SUCCESS;
    }
}

/// @brief Verifies password based on hash and salt
/// @param password
/// @param hash
bool VerifyPassword(const std::string& password, const std::string& hash, const std::string& salt)
{
    return simple_sha256_hash(password + salt) == hash;
}

/// @brief Generate random string
/// @param len
/// @return std::string
std::string gen_random(const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    
    return tmp_s;
}
#pragma once
#include "../Database/database.hpp"
#include <vector>

enum class CreateAccountResult {
    SUCCESS,
    FIELDS_EMPTY,
    INTERNAL_ERROR
};
class UserRepo
{
    public:
    std::unique_ptr<User> GetUserById(uint32_t uid);
    uint32_t InsertUser(const User& user, bool overwrite= false);
};


struct HashedPassword {
    std::string hash;
    std::string salt;
};

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

HashedPassword HashPassword(const std::string& password);
bool VerifyPassword(const std::string& password, const std::string& hash, const std::string& salt);
std::string gen_random(const int len);
LoginResponse Login(const std::string& login, const std::string& password);
RegisterResult Register(const std::string& login, const std::string& name, const std::string& password);
std::vector<Account> GetUserAccounts(uint32_t uid);
CreateAccountResult CreateUserAccount(const uint32_t user_id, const std::string& name, int phone_number);
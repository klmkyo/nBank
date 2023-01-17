#include "../Database/database.hpp"
#pragma once

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

enum class RegisterResult {
    SUCCESS,
    USER_EXISTS,
    INTERNAL_ERROR
};

HashedPassword HashPassword(const std::string& password);
bool VerifyPassword(const std::string& password, const std::string& hash, const std::string& salt);
std::string gen_random(const int len);
LoginResult Login(const std::string& login, const std::string& password);
RegisterResult Register(const std::string& login, const std::string& password);
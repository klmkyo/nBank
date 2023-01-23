#pragma once
#include <vector>
#include "../Database/Database.hpp"
#include "../Utils/Utils.hpp"

enum class CreateAccountResult {
    SUCCESS,
    FIELDS_EMPTY,
    INTERNAL_ERROR,
    PHONE_NUMBER_EXISTS
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

LoginResponse Login(const std::string& login, const std::string& password);
RegisterResult Register(const std::string& login, const std::string& name, const std::string& password);
std::vector<Account> GetUserAccounts(uint32_t uid);
CreateAccountResult CreateUserAccount(const uint32_t user_id, const std::string& name, int phone_number);
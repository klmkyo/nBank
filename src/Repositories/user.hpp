#include "../Database/database.hpp"

class UserRepo
{
    public:
    std::unique_ptr<User> GetUserById(uint32_t uid);
    uint32_t InsertUser(const User& user, bool overwrite= false);
};
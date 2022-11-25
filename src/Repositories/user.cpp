#include "user.hpp"

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
uint32_t UserRepo::InsertUser(User* user, bool overwrite)
{
    auto _user = UserRepo::GetUserById(user->id);
    uint32_t id = -1;
    if (_user && overwrite){
        try{
            Database::getStorage()->replace(*user);
            id = user->id;
        }catch(...) {
            // TODO: Exception handling
            return -1;
        }
    } else {
        try{
            id = Database::getStorage()->insert(*user);
        }catch(...) {
            // TODO: Exception handling
            return -1;
        }
    }
    return id;
}
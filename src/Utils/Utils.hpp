#pragma once
#include <string>

namespace Utils {
    namespace Type {
        struct HashedPassword {
            std::string hash;
            std::string salt;
        };
    }
    
    Type::HashedPassword HashPassword(const std::string& password);
    std::string gen_random(const int len);
    std::string gen_random_num(const int len);
    bool verify_password(const std::string& password, const std::string& hash, const std::string& salt);
    std::string simple_sha256_hash(const std::string& input);
    std::string double_to_string(double d);
}
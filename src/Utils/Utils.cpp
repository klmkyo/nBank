#include <iomanip>
#include <sstream>
#include "Utils.hpp"
#include "time.h"
#include "Utils/sha256.hpp"

namespace Utils {
    
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


    std::string simple_sha256_hash(const std::string& input) {
        SHA256 sha;
        
        sha.update(input);
        uint8_t* digest = sha.digest();
        std::string output = sha.toString(digest);

        delete[] digest; // Don't forget to free the digest!

        return output;
    }

    /// @brief Verifies password based on hash and salt
    /// @param password
    /// @param hash
    bool verify_password(const std::string& password, const std::string& hash, const std::string& salt)
    {
        return simple_sha256_hash(password + salt) == hash;
    }

    /// @brief Hash password using sha256
    /// @param password
    /// @return HashedPassword
    Type::HashedPassword HashPassword(const std::string& password)
    {
        srand(time(NULL));
        Type::HashedPassword hp;
        // random salt
        hp.salt = Utils::gen_random(16);
        // hash password
        hp.hash = Utils::simple_sha256_hash(password + hp.salt);
        return hp;
    }

    /// @brief Returns a string from double with 2 decimal places
    /// @param d
    /// @return std::string
    std::string double_to_string(double d)
    {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << d;
        return ss.str();
    }
}
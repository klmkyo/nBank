#pragma once
#include "user.hpp"
#include "account.hpp"

template <class T>
class Repo {
public:
    static std::unique_ptr<T> GetById(uint32_t id) {
        if(Repo<T>::Exist(id)) {
            return Database::getStorage()->get_pointer<T>(id);
        }   
        return nullptr;
    }

    static bool Exist(uint32_t id) { // watchout maybe wrong
        return Database::getStorage()->get_pointer<T>(id) ? true : false; 
    }

    // static std::vector<T> GetAllMatching(int T::* a, int f) {
    //     return Database::getStorage()->get_all<T>(sqlite_orm::where(sqlite_orm::c(a) == f));
    // }

    static uint32_t Insert(const T& t) {
        uint32_t id = -1;
      
        try {
            id = Database::getStorage()->insert(t);
        } catch(...) {
            // TODO: Exception handling
            return -1;
        }
        
        return id;
    }

    static uint32_t Replace(const T& t) {
        uint32_t id = -1;
        //if (Repo<T>::Exist(id)) {
            try {
                Database::getStorage()->replace(t);
                id = t.id;
            } catch(...) {
                return -1;
            }
        //}
        return id;
    }
    
    static bool Update(T& t) {
        uint32_t id = -1;
        //if (Repo<T>::Exist(id)) {
            try {
                Database::getStorage()->update(t);
            } catch(...){
                return false;
            }
        //}

        return 1;
    }
};





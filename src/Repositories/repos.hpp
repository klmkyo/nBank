#pragma once
#include "user.hpp"
#include "account.hpp"

template <class T>
class Repo
{
public:
    static std::unique_ptr<T> GetById(uint32_t id)
    {
        if (auto acc = Database::getStorage()->get_pointer<T>(id)) {
            return acc;
        } else {
            return nullptr;
        }
    }

    static uint32_t Insert(const T& t) {
        auto _account = Repo<T>::GetById(t.id);
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
        auto _account = Repo<T>::GetById(t.id);
        uint32_t id = -1;
        if (_account) {
            try{
                Database::getStorage()->replace(t);
                id = t.id;
            }catch(...) {
                // TODO: Exception handling
                return -1;
            }
        }
        return id;
    }
    
    static bool Update(const T& t)
    {
        if (auto _account = Repo<T>::GetById(t.id)){
            try{
                Database::getStorage()->update(t);
            } catch(...){
                return false;
            }
            return true;
        } else {
            return false;
        }
    }
};





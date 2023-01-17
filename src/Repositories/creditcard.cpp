#include "creditcard.hpp"

/// @brief Search for card by id
/// @param id
/// @return unique_ptr<CreditCard> if found, otherwise nullptr
std::unique_ptr<CreditCard> CreditCardRepo::GetCreditCardById(uint32_t id)
{
    if (auto card = Database::getStorage()->get_pointer<CreditCard>(id)){
        return card;
    } else {
        return nullptr;
    }
}

/// @brief Insert new CreditCard into the database, use overwrite if you want to replace existing CreditCard
/// @param CreditCard 
/// @param overwrite (optional, default: false)
/// @return ID of created CreditCard (if failed, returns -1)
uint32_t CreditCardRepo::InsertCreditCard(const CreditCard& CreditCard, bool overwrite)
{
    auto _CreditCard = CreditCardRepo::GetCreditCardById(CreditCard.id);
    uint32_t id = -1;
    if (_CreditCard && overwrite){
        try{
            Database::getStorage()->replace(CreditCard);
            id = CreditCard.id;
        }catch(...) {
            // TODO: Exception handling
            return -1;
        }
    } else {
        try{
            id = Database::getStorage()->insert(CreditCard);
        }catch(...) {
            // TODO: Exception handling
            return -1;
        }
    }
    return id;
}

bool CreditCardRepo::UpdateCreditCard(const CreditCard& CreditCard)
{
    if (auto _CreditCard = CreditCardRepo::GetCreditCardById(CreditCard.id)){
        try{
            Database::getStorage()->update(CreditCard);
        } catch(...){
            return false;
        }
        return true;
    } else {
        return false; // CreditCard not found
    }
}
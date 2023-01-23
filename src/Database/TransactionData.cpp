#include "Database/TransactionData.hpp"

TransactionData::TransactionData(uint32_t sender_id, uint32_t recipent_id, double amount): 
                  sender_id(sender_id), recipent_id(recipent_id), amount(amount) {
}


TransactionData::TransactionData(const Account& sender, const Account& recipent, double amount) : amount(amount) {
    sender_id = sender.user_id;
    recipent_id = recipent.user_id;
} 

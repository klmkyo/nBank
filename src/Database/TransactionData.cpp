#include "Database/TransactionData.hpp"

TransactionData::TransactionData(uint32_t sender_account_id, uint32_t recipient_account_id, double amount): 
                  sender_account_id(sender_account_id), recipient_account_id(recipient_account_id), amount(amount) {
}


TransactionData::TransactionData(const Account& sender, const Account& recipent, double amount) : amount(amount) {
    sender_account_id = sender.id;
    recipient_account_id = recipent.id;
} 

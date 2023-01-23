#pragma once
#include <iostream>
#include <sstream>
#include "Logic/transfer.hpp"


// Do ilu pln płatność kartą nie wymaga podawania pinu
#define NO_PIN_LIMIT 10.0

// Zwykły przelew na numer konta (numer karty połączonej z kontem)
class DirectTransfer : public Transfer {
    private:
    int card_number;
    public:
    DirectTransfer(Account* account, double amount, int card_number) 
    : Transfer(account, amount) {
        using namespace sqlite_orm;
        this->card_number = card_number;
        auto cards = Database::getStorage()->get_all<CreditCard>(where(c(&CreditCard::number) == card_number));
        if (cards.size() < 1)
            return; // card not found
        recipient = Repo<Account>::GetById(cards[cards.size() - 1].account_id).release();
    }
    std::string GetLogMessage(bool success) {
        if (this->account == nullptr)
            return "Direct transfer failed (System Error)";
        std::ostringstream oss;
        if (success){
            oss << "Direct transfer of " << std::abs(this->amount)
             << " PLN from account '" << account->name << "(id:"<<account->id<<")' to '"
             << recipient->name << "(id:"<<recipient->id<<")' successful!";
        } else {
            oss << "Direct transfer of " << std::abs(this->amount)
            << " PLN from account '" << account->name << "(id:"<<account->id<<
            ")' to a card number '" << card_number << "' failed (" << resultString << ")";
        }
        
        return oss.str();
    };
    ~DirectTransfer(){
        // recipient obj created in constructor
        delete this->recipient;
        this->recipient = nullptr;
    }
};

// Przelew na telefon (BLIK)
class BLIKTransfer : public Transfer {
    private:
    int phone_number;
    public:
    BLIKTransfer(Account* account, double amount, int phone_number) 
    : Transfer(account, amount) {
        using namespace sqlite_orm;
        this->phone_number = phone_number;
        auto accounts = Database::getStorage()->get_all<Account>(where(c(&Account::phone_number) == phone_number));
        if (accounts.size() < 1)
            return; // account not found
        recipient = Repo<Account>::GetById(accounts[accounts.size() - 1].id).release();
    }
    std::string GetLogMessage(bool success) {
        if (this->account == nullptr)
            return "BLIK transfer failed (System Error)";
        std::ostringstream oss;
        if (success){
            oss << "BLIK transfer of " << std::abs(this->amount)
             << " PLN from account '" << account->name << "(id:"<<account->id<<")' to '"
             << recipient->name << "(id:"<<recipient->id<<")' successful!";
        } else {
            oss << "BLIK transfer of " << std::abs(this->amount)
            << " PLN from account '" << account->name << "(id:"<<account->id<<
            ")' to a phone number '" << phone_number << "' failed (" << resultString << ")";
        }
        
        return oss.str();
    };
    ~BLIKTransfer(){
        // recipient obj created in constructor
        delete this->recipient;
        this->recipient = nullptr;
    }
};

struct CreditInput {
    int number, cvv, exp_month, exp_year, pin;

    CreditInput(int number, int cvv, int exp_month, int exp_year, int pin)
    : number(number), cvv(cvv), exp_month(exp_month), exp_year(exp_year), pin(pin) {}
};

// Płatność kartą
class CardTransaction : public Transaction {
    public:
    CardTransaction(const CreditInput& input, double amount)
    : Transaction(-amount)
    {
        using namespace sqlite_orm;
        auto cards = Database::getStorage()->get_all<CreditCard>(where(c(&CreditCard::number) == input.number));
        if (cards.size() < 1)
            return;
        CreditCard c = cards[cards.size()-1];
        if (input.cvv != c.cvv || input.exp_month != c.expiration_month
            || input.exp_year != c.expiration_year)
            return;
        if (amount >= NO_PIN_LIMIT) {
            this->requirePin = true;
            this->wrongPin = input.pin != c.pin;
        }
        account = Repo<Account>::GetById(c.account_id).release();

    }
    std::string GetLogMessage(bool success) {
        if (this->account == nullptr)
            return "Card payment failed (System Error)";
        std::ostringstream oss;
        if (success){
            oss << "Card payment of " << std::abs(this->amount)
             << " PLN from account '" << account->name << "(id:"<<account->id<<")' successful!";
        } else {
            oss << "Card payment of " << std::abs(this->amount)
            << " PLN from account '" << account->name << "(id:"<<account->id<<
            ")' failed (" << resultString << ")";
        }
        
        return oss.str();
    };
    ~CardTransaction(){
        // account obj created in constructor
        delete this->account;
        this->account = nullptr;
    }
    bool doesRequirePin() const {
        return this->requirePin;
    }
};

// Wypłata (z karty)
class WithdrawTransaction : public Transaction {
    public:
    WithdrawTransaction(const CreditInput& input, double amount)
    : Transaction(-amount)
    {
        using namespace sqlite_orm;
        auto cards = Database::getStorage()->get_all<CreditCard>(where(c(&CreditCard::number) == input.number));
        if (cards.size() < 1)
            return;
        CreditCard c = cards[cards.size()-1];
        if (input.cvv != c.cvv || input.exp_month != c.expiration_month
            || input.exp_year != c.expiration_year)
            return;
        this->requirePin = true;
        this->wrongPin = input.pin != c.pin;
        account = Repo<Account>::GetById(c.account_id).release();

    }
    std::string GetLogMessage(bool success) {
        if (this->account == nullptr)
            return "Withdraw transaction failed (System Error)";
        std::ostringstream oss;
        if (success){
            oss << "Withdraw transaction of " << std::abs(this->amount)
             << " PLN from account '" << account->name << "(id:"<<account->id<<")' successful!";
        } else {
            oss << "Withdraw transaction of " << std::abs(this->amount)
            << " PLN from account '" << account->name << "(id:"<<account->id<<
            ")' failed (" << resultString << ")";
        }
        
        return oss.str();
    };
    ~WithdrawTransaction(){
        // account obj created in constructor
        delete this->account;
        this->account = nullptr;
    }
};

// Wpłata (na karte)
class DepositTransaction : public Transaction {
    public:
    DepositTransaction(const CreditInput& input, double amount)
    : Transaction(amount)
    {
        using namespace sqlite_orm;
        auto cards = Database::getStorage()->get_all<CreditCard>(where(c(&CreditCard::number) == input.number));
        if (cards.size() < 1)
            return;
        CreditCard c = cards[cards.size()-1];
        if (input.cvv != c.cvv || input.exp_month != c.expiration_month
            || input.exp_year != c.expiration_year)
            return;
        this->requirePin = true;
        this->wrongPin = input.pin != c.pin;
        account = Repo<Account>::GetById(c.account_id).release();
    }
    std::string GetLogMessage(bool success) {
        if (this->account == nullptr)
            return "Deposit transaction failed (System Error)";
        std::ostringstream oss;
        if (success){
            oss << "Deposit transaction of " << std::abs(this->amount)
             << " PLN to account '" << account->name << "(id:"<<account->id<<")' successful!";
        } else {
            oss << "Deposit transaction of " << std::abs(this->amount)
            << " PLN to account '" << account->name << "(id:"<<account->id<<
            ")' failed (" << resultString << ")";
        }
        
        return oss.str();
    };
    ~DepositTransaction(){
        // account obj created in constructor
        delete this->account;
        this->account = nullptr;
    }
};

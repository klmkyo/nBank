#pragma once
#include <iostream>
#include "Logic/transfer.hpp"

// Do ilu pln płatność kartą nie wymaga podawania pinu
#define NO_PIN_LIMIT 10.0

// Zwykły przelew na numer konta (numer karty połączonej z kontem)
class DirectTransfer : public Transfer {
    public:
    DirectTransfer(Account* account, double amount, int card_number) 
    : Transfer(account, amount) {
        using namespace sqlite_orm;
        auto cards = Database::getStorage()->get_all<CreditCard>(where(c(&CreditCard::number) == card_number));
        if (cards.size() < 1)
            return; // card not found
        recipient = Repo<Account>::GetById(cards[cards.size() - 1].account_id).release();
    }
    ~DirectTransfer(){
        // recipient obj created in constructor
        delete this->recipient;
        this->recipient = nullptr;
    }
};

// Przelew na telefon (BLIK)
class BLIKTransfer : public Transfer {
    public:
    BLIKTransfer(Account* account, double amount, int phone_number) 
    : Transfer(account, amount) {
        using namespace sqlite_orm;
        auto accounts = Database::getStorage()->get_all<Account>(where(c(&Account::phone_number) == phone_number));
        if (accounts.size() < 1)
            return; // account not found
        recipient = Repo<Account>::GetById(accounts[accounts.size() - 1].id).release();
    }
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
    ~DepositTransaction(){
        // account obj created in constructor
        delete this->account;
        this->account = nullptr;
    }
};

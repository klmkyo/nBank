#include <iostream>
#include <sqlite3.h>
#include "Database/Database.hpp"
#include "Repositories/repos.hpp"
#include "Repositories/user.hpp"
#include "UI/ui.hpp"
#include "UI/transferui.hpp"

int main() {
    Database::getStorage()->sync_schema();

    /* //przelew blik między 2 kontami
    uint32_t test1 = Repo<Account>::Insert(Account{-1, -1, "test1", 15.0, 1337});
    uint32_t test2 = Repo<Account>::Insert(Account{-1, -1, "test2", 66.0, 4752});
    Account* acc = Repo<Account>::GetById(test1).release();
    Account* acc2 = Repo<Account>::GetById(test2).release();
    std::cout << *acc << std::endl;
    std::cout << *acc2 << std::endl;
    // 5pln z konta acc(ptr) na podany numer tel(int)
    BLIKTransfer t = BLIKTransfer(acc, 5, 122);
    TransactionResult tr;
    t.Execute(tr); // wykonuje przelew
    delete acc, acc2;
    acc2 = Repo<Account>::GetById(test2).release(); // pobiera zaktualizowane dane z bazy
    return 0; */
    

    // do debugowania paneli przelewów
//    uint32_t test1 = Repo::Account()->InsertAccount(Account{-1, -1, "test1", 15.0, 1337});
//    Account* acc = Repo::Account()->GetAccountById(test1).release();
//    TransferPanel(*acc);
    

    // tworzy nowego Usera i nowe konto
    // uint32_t newuser = Repo::User()->InsertUser(User{-1, "admin", "admin", "0123456789abcdef", "admin"});
    // if (newuser != -1){
    //         // zwraca dane nowego usera
    //     std::cout << *(Repo::User()->GetUserById(newuser)) << std::endl;
    //         // tworzy nowe konto dla usera
    //     uint32_t newaccount = Repo::Account()->InsertAccount(Account{-1, (int)newuser, "konto 1", 15.0});
    //     if (newaccount != -1){
    //             // zwraca dane nowego konta
    //         std::unique_ptr<Account> acc = Repo::Account()->GetAccountById(newaccount);
    //         std::cout << *acc << std::endl;
    //             // dodaje 15 zł do konta
    //         acc->balance += 15;
    //         Repo::Account()->UpdateAccount(*acc);
    //         std::cout << *Repo::Account()->GetAccountById(acc->id) << std::endl;
    //     }
    // }
    // --------------
    
    // uwaga: ftxui nie ma świetnej dokumentacji odnośnie tego jak prawdłowo wyświetlać
    // różne ekrany/strony, więc mogę to robić w niestandardowy i pobugowany sposób
    // "u mnie działa", ale jeśli u was by tak nie było to dajcie znać
    
    // tworzy pustego usera, do ktorego zostanie wczytany zalogowany user
    User user{};
    
    // ekran logowania
    // LoginScreen(user);

    // dla debugu, przelew między kontami w tym samym userze
    auto account_source = Database::getStorage()->get<Account>(1);

    DirectTransfer t = DirectTransfer(&account_source, 5.0, 12546724);
    TransactionResult tr;
    t.Execute(tr); // wykonuje przelew
    
    // dla debugu, weź pierwszego usera z bazy
    // auto users = Database::getStorage()->get_all<User>();
    // if (users.size() > 0){
    //     user = users[0];
    // }

    // wcisniecie ctrl+c w trakcie LoginScreen() wychodzi z funkcji zamiast zamykac program
    // dlatego trzeba sprawdzic czy user zostal zalogowany
    if(user.id){
        // panel glowny
        Dashboard(user);
    }

    return 0;
}
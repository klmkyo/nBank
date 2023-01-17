#include <iostream>
#include <sqlite3.h>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/string.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <Database/database.hpp>
#include <Repositories/repos.hpp>

int main() {
    std::cout << "Hello World!" << std::endl;

    //
    // Przykład korzystania z repository pattern
    //
    Database::getStorage()->sync_schema();
        // tworzy nowego Usera i nowe konto
    uint32_t newuser = Repo::User()->InsertUser(User{-1, "nowyUzytkownik"});
    if (newuser != -1){
            // zwraca dane nowego usera
        std::cout << *(Repo::User()->GetUserById(newuser)) << std::endl;
            // tworzy nowe konto dla usera
        uint32_t newaccount = Repo::Account()->InsertAccount(Account{-1, (int)newuser, "konto 1", 15.0});
        if (newaccount != -1){
                // zwraca dane nowego konta
            std::unique_ptr<Account> acc = Repo::Account()->GetAccountById(newaccount);
            std::cout << *acc << std::endl;
                // dodaje 15 zł do konta
            acc->balance += 15;
            Repo::Account()->UpdateAccount(*acc);
            std::cout << *Repo::Account()->GetAccountById(acc->id) << std::endl;
        }
    }
    // --------------

    using namespace ftxui;
    // bank dashboard main loop
    std::vector<std::string> entries = {
        "tribute",     "clearance", "ally",        "bend",        "electronics",
        "module",      "era",       "cultural",    "sniff",       "nationalism",
        "negotiation", "deliver",   "figure",      "east",        "tribute",
        "clearance",   "ally",      "bend",        "electronics", "module",
        "era",         "cultural",  "sniff",       "nationalism", "negotiation",
        "deliver",     "figure",    "east",        "tribute",     "clearance",
        "ally",        "bend",      "electronics", "module",      "era",
        "cultural",    "sniff",     "nationalism", "negotiation", "deliver",
        "figure",      "east",
    };
    
    int selected_1 = 0;
    int selected_2 = 0;
    int selected_3 = 0;
    int selected_4 = 0;
    
    auto layout = Container::Vertical({
        Container::Horizontal({
            Dropdown(&entries, &selected_1),
            Dropdown(&entries, &selected_2),
        }),
        Container::Horizontal({
            Dropdown(&entries, &selected_3),
            Dropdown(&entries, &selected_4),
        }),
    });
    
    auto screen = ScreenInteractive::FitComponent();
    screen.Loop(layout);
    return 0;
}
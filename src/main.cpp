#include <iostream>
#include <sqlite3.h>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/string.hpp>
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

    auto summary = [&] {
        auto content = vbox({
            hbox({text(L"- done:   "), text(L"3") | bold}) | color(Color::Green),
            hbox({text(L"- active: "), text(L"2") | bold}) | color(Color::RedLight),
            hbox({text(L"- queue:  "), text(L"9") | bold}) | color(Color::Red),
        });
        return window(text(L" Summary "), content);
    };

    auto document =  //
        vbox({
            hbox({
                summary(),
                summary(),
                summary() | flex,
            }),
            summary(),
            summary(),
        });

    // Limit the size of the document to 80 char.
    document = document | size(WIDTH, LESS_THAN, 80);

    auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
    Render(screen, document);

    std::cout << screen.ToString() << '\0' << std::endl;
}
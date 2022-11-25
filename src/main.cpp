#include <iostream>
#include <sqlite3.h>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/string.hpp>
#include <Database/database.hpp>
#include <Repositories/user.hpp>

int main() {
    std::cout << "Hello World!" << std::endl;

    // Database service & repo test
    Database::getStorage()->sync_schema();
    auto repo = std::make_shared<UserRepo>();
            // tworzy nowego Usera z nazwą test123
    uint32_t newuser = repo->InsertUser(new User{-1, "test123"});
            // zwraca nazwe nowego usera
    std::cout << repo->GetUserById(newuser)->name << std::endl;
    // -----

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
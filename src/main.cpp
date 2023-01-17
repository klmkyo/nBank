#include <iostream>
#include <sqlite3.h>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/string.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <Database/database.hpp>
#include <Repositories/repos.hpp>
#include <Repositories/user.hpp>
#include <ftxui/component/component_options.hpp>  // for InputOption

int main() {

    //
    // Przykład korzystania z repository pattern
    //
    Database::getStorage()->sync_schema();

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

    using namespace ftxui;
    std::string login;
    std::string password;

    Component input_first_name = Input(&login, "login");
    InputOption password_option;
    password_option.password = true;
    Component input_password = Input(&password, "hasło", password_option);

    Component button = Button("Zaloguj się", [&] {
        std::cout << "Login: " << login << std::endl;
        std::cout << "Password: " << password << std::endl;
    });

    auto component = Container::Vertical({
        input_first_name,
        input_password,
        button,
    });

    auto renderer = Renderer(component, [&] {
        return window(text("Logowanie"), vbox({
                hbox(
                    input_first_name->Render() | borderLight,
                    input_password->Render() | borderLight
                ),
                button->Render()
            }));
    });

    auto screen = ScreenInteractive::TerminalOutput();
    screen.Loop(renderer);
    return 0;
}
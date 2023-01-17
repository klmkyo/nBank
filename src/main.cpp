#include <iostream>
#include <sqlite3.h>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/string.hpp>
#include <ftxui/component/component_options.hpp>  // for InputOption
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <Database/database.hpp>
#include <Repositories/repos.hpp>
#include <Repositories/user.hpp>

using namespace ftxui;

Component ModalComponent(std::function<void()> do_nothing, std::function<void()> hide_modal);

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

    std::string login;
    std::string password;

    Component input_first_name = Input(&login, "login");
    InputOption password_option;
    password_option.password = true;
    Component input_password = Input(&password, "hasło", password_option);

    bool show_modal = false;

    Component registerbutton = Button("Zarejestruj się", [&] {
        auto status = Register(login, password);
        switch (status)
        {
            case RegisterStatus::SUCCESS:
            std::cout << "Zarejestrowano" << std::endl;
            break;
            case RegisterStatus::USER_EXISTS:
            std::cout << "Użytkownik już istnieje" << std::endl;
            break;
            case RegisterStatus::INTERNAL_ERROR:
            std::cout << "Błąd wewnętrzny" << std::endl;
            break;
        }
    });

    Component loginbutton = Button("Zaloguj się", [&] {
        auto result = Login(login, password);
        switch (result.status)
        {
            case LoginStatus::SUCCESS:
            std::cout << "Zalogowano" << std::endl;
            break;
            case LoginStatus::WRONG_PASSWORD:
            std::cout << "Błędne hasło" << std::endl;
            break;
            case LoginStatus::USER_NOT_FOUND:
            std::cout << "Użytkownik nie istnieje" << std::endl;
            break;
            case LoginStatus::INTERNAL_ERROR:
            std::cout << "Błąd wewnętrzny" << std::endl;
            break;
        }
    });

    auto component = Container::Vertical({
        input_first_name,
        input_password,
        loginbutton,
    });

    auto renderer = Renderer(component, [&] {
        return 
            center(
                window(text("Logowanie"), vbox(
                    vbox(
                        input_first_name->Render() | borderLight,
                        input_password->Render() | borderLight
                    ),
                    separator(),
                    loginbutton->Render()
                    )
                ) | size(WIDTH, EQUAL, 60)
            );
    });

    auto hide_modal = [&] { show_modal = false; };
    auto do_nothing = [&] {};
    auto modal_component = ModalComponent(do_nothing, hide_modal);

    renderer |= Modal();

    auto screen = ScreenInteractive::Fullscreen();
    screen.Loop(renderer);
    return 0;
}

// Definition of the modal component. The details are not important.
Component ModalComponent(std::function<void()> do_nothing,
                         std::function<void()> hide_modal) {
  auto component = Container::Vertical({
      Button("Do nothing", do_nothing),
      Button("Quit modal", hide_modal),
  });
  // Polish how the two buttons are rendered:
  component |= Renderer([&](Element inner) {
    return vbox({
               text("Modal component "),
               separator(),
               inner,
           })
           | size(WIDTH, GREATER_THAN, 30)
           | border;
  });
  return component;
}
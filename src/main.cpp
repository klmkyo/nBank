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

// TODO zamiast nestować tworzenie roznych screenów,
// nalezy zrobic osobne componenty i je renderowac

void LoginScreen();
void Dashboard(User user);
void Dialog(const std::string& message);

enum class Menu {
    LOGIN,
    REGISTER,
    DIALOG,
};

int main() {
    Database::getStorage()->sync_schema();

    /* //przelew blik między 2 kontami
    uint32_t test1 = Repo::Account()->InsertAccount(Account{-1, -1, "test1", 15.0, 1337});
    uint32_t test2 = Repo::Account()->InsertAccount(Account{-1, -1, "test2", 66.0, 4752});
    Account* acc = Repo::Account()->GetAccountById(test1).release();
    Account* acc2 = Repo::Account()->GetAccountById(test2).release();
    std::cout << *acc << std::endl;
    std::cout << *acc2 << std::endl;
    // 5pln z konta acc(ptr) na podany numer tel(int)
    BLIKTransfer* t = new BLIKTransfer(acc, 5, acc2->phone_number);
    TransactionResult tr;
    t->Execute(tr); // wykonuje przelew
    delete t, acc, acc2;
    acc2 = Repo::Account()->GetAccountById(test2).release(); // pobiera zaktualizowane dane z bazy
    std::cout << *acc << std::endl;
    std::cout << *acc2 << std::endl;
    return 0;
    */
    

      

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
    
    LoginScreen();

    return 0;
}

void LoginScreen() {
    std::string login;
    std::string password;

    Component input_first_name = Input(&login, "login");
    InputOption password_option;
    password_option.password = true;
    Component input_password = Input(&password, "hasło", password_option);

    Component loginbutton = Button("Zaloguj się", [&] {
        auto result = Login(login, password);
        switch (result.status)
        {
            case LoginResult::SUCCESS:
                Dashboard(result.user);
                return result.user;
                break;
            case LoginResult::WRONG_PASSWORD:
                Dialog("Niepoprawne hasło");
                break;
            case LoginResult::USER_NOT_FOUND:
                Dialog("Niepoprawny login");
                break;
            case LoginResult::INTERNAL_ERROR:
                Dialog("Błąd wewnętrzny");
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

    auto screen = ScreenInteractive::Fullscreen();
    screen.Loop(renderer);
}

void Dialog(const std::string& message)
{
    auto screen = ScreenInteractive::Fullscreen();

    auto ok_button = Button("OK", [&] {
        screen.ExitLoopClosure()();
    });

    auto component = Container::Vertical({
        ok_button
    });

    auto renderer = Renderer(component, [&] {
        return 
            center(
                vbox(
                    vbox(
                        text(message)
                    ),
                    separator(),
                    ok_button->Render()
                 ) | border | size(WIDTH, EQUAL, 60)
            );
    });

    screen.Loop(renderer);
}

void Dashboard(User user)
{
    auto screen = ScreenInteractive::Fullscreen();

    auto logout_button = Button("Wyloguj", [&] {
        screen.ExitLoopClosure()();
    });

    auto component = Container::Vertical({
        logout_button
    });

    auto renderer = Renderer(component, [&] {
        return 
            center(
                window(text("Dialog"), vbox(
                    vbox(
                        text("Witaj " + user.login)
                    ),
                    separator(),
                    logout_button->Render()
                    )
                ) | size(WIDTH, EQUAL, 60)
            );
    });

    screen.Loop(renderer);
}
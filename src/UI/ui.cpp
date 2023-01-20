#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/string.hpp>
#include <ftxui/component/component_options.hpp>  // for InputOption
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <Repositories/user.hpp>
#include <Repositories/repos.hpp>
#include "ui.hpp"

// TODO display logo
const std::vector<std::string> LOGO = {
    R"(          $$$$$$$\                      $$\       )",
    R"(          $$  __$$\                     $$ |      )",
    R"($$$$$$$\  $$ |  $$ | $$$$$$\  $$$$$$$\  $$ |  $$\ )",
    R"($$  __$$\ $$$$$$$\ | \____$$\ $$  __$$\ $$ | $$  |)",
    R"($$ |  $$ |$$  __$$\  $$$$$$$ |$$ |  $$ |$$$$$$  / )",
    R"($$ |  $$ |$$ |  $$ |$$  __$$ |$$ |  $$ |$$  _$$<  )",
    R"($$ |  $$ |$$$$$$$  |\$$$$$$$ |$$ |  $$ |$$ | \$$\ )",
    R"(\__|  \__|\_______/  \_______|\__|  \__|\__|  \__|)"
};

constexpr int LOGO_HEIGHT = 8;

using namespace ftxui;

/// @brief Loguje użytownika, zwraca jego dane do user
/// @param user - zwracany użytkownik
void LoginScreen(User& user) {
    auto screen = ScreenInteractive::Fullscreen();

    Elements elements;
    // go through each line of logo, and append text(line) to elements
    for (auto& line : LOGO) {
        elements.push_back(text(line));
    }

    std::string login;
    std::string password;

    Component input_login = Input(&login, "login");
    InputOption password_option;
    password_option.password = true;
    Component input_password = Input(&password, "hasło", password_option);

    Component loginbutton = Button("Zaloguj się", [&] {
        auto result = Login(login, password);
        switch (result.status)
        {
            case LoginResult::SUCCESS:
                user = result.user;
                screen.ExitLoopClosure()();
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

    Component registerbutton = Button("Utwórz Konto", [&] {
        RegisterScreen(login, password);
    });

    auto component = Container::Vertical({
        input_login,
        input_password,
        loginbutton,
        registerbutton
    });

    auto renderer = Renderer(component, [&] {
        return 
            center(
                vbox(
                    vbox({
                        elements | center
                    }) | size(HEIGHT, EQUAL, LOGO_HEIGHT),
                    text("") | center,
                    text("") | center,
                    window(text("Logowanie"), vbox(
                    vbox(
                        input_login->Render() | borderLight,
                        input_password->Render() | borderLight
                    ),
                    separator(),
                    loginbutton->Render(),
                    registerbutton->Render()
                    )
                ) | size(WIDTH, EQUAL, 60)
                )
            );
    });

    screen.Loop(renderer);
}

/// @brief Rejestruje użytkownika
/// @param login - opcjonalny login, który zostanie wpisany do pola
/// @param password - opcjonalne hasło, które zostanie wpisane do pola
void RegisterScreen(std::string login = "", std::string password = "") {
    auto screen = ScreenInteractive::Fullscreen();

    std::string name;
    std::string password2;
    std::string phone;

    Component input_login = Input(&login, "login");
    Component input_name = Input(&name, "imię");
    InputOption password_option;
    password_option.password = true;
    Component input_password = Input(&password, "hasło", password_option);
    Component input_password2 = Input(&password2, "powtórz hasło", password_option);

    Component registerbutton = Button("Zarejestruj się", [&] {
        if (password != password2) {
            Dialog("Hasła nie są takie same");
            return;
        }
        auto result = Register(login, name, password);
        switch (result)
        {
            case RegisterResult::SUCCESS:
                Dialog("Zarejestrowano pomyślnie");
                screen.ExitLoopClosure()();
                break;
            case RegisterResult::USER_EXISTS:
                Dialog("Użytkownik o takim loginie już istnieje");
                break;
            case RegisterResult::INTERNAL_ERROR:
                Dialog("Błąd wewnętrzny");
                break;
            case RegisterResult::FIELDS_EMPTY:
                Dialog("Pola nie mogą być puste");
                break;
        }
    });

    Component cancelbutton = Button("Anuluj", [&] {
        screen.ExitLoopClosure()();
    });

    auto component = Container::Vertical({
        input_name,
        input_login,
        input_password,
        input_password2,
        cancelbutton,
        registerbutton,
    });

    auto renderer = Renderer(component, [&] {
        return 
            center(
                window(text("Rejestracja"), vbox(
                    vbox(
                        input_name->Render() | borderLight,
                        input_login->Render() | borderLight,
                        input_password->Render() | borderLight,
                        input_password2->Render() | borderLight
                    ),
                    separator(),
                    hbox(
                        cancelbutton->Render(),
                        registerbutton->Render() | flex
                    )
                    )
                ) | size(WIDTH, EQUAL, 60)
            );
    });

    screen.Loop(renderer);
}

/// @brief Wyświetla dialog z wiadomością
/// @param message - wiadomość do wyświetlenia
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

void CreateAccountPanel(User& user)
{
    auto screen = ScreenInteractive::Fullscreen();

    std::string name;
    std::string balance;
    std::string phone_number;

    Component input_name = Input(&name, "Nazwa konta");
    Component input_balance = Input(&balance, "Saldo początkowe");
    Component input_phone_number = Input(&phone_number, "Numer telefonu");

    Component create_button = Button("Utwórz", [&] {
        if (name.empty()) {
            Dialog("Nazwa konta nie może być pusta");
            return;
        }
        if (balance.empty()) {
            Dialog("Saldo nie może być puste");
            return;
        }
        if (phone_number.empty()) {
            Dialog("Numer telefonu nie może być pusty");
            return;
        }

        // bezpieczne rzutowanie na double balance i phone_number
        double balance_value;
        try {
            balance_value = std::stod(balance);
        } catch (const std::invalid_argument& e) {
            Dialog("Saldo musi być liczbą");
            return;
        } catch (const std::out_of_range& e) {
            Dialog("Saldo jest zbyt duże");
            return;
        }

        int phone_number_value;
        try {
            phone_number_value = std::stoi(phone_number);
        } catch (const std::invalid_argument& e) {
            Dialog("Numer telefonu musi być liczbą");
            return;
        } catch (const std::out_of_range& e) {
            Dialog("Numer telefonu jest zbyt duży");
            return;
        }

        auto result = CreateUserAccount(user.id, name, balance_value, phone_number_value);
        switch (result) {
            case CreateAccountResult::SUCCESS:
                Dialog("Utworzono konto!");
                screen.ExitLoopClosure()();
                break;
            case CreateAccountResult::INTERNAL_ERROR:
                Dialog("Błąd wewnętrzny");
                break;
            case CreateAccountResult::FIELDS_EMPTY:
                Dialog("Pola nie mogą być puste");
                break;
        }
    });

    Component cancel_button = Button("Anuluj", [&] {
        screen.ExitLoopClosure()();
    });

    auto component = Container::Vertical({
        input_name,
        input_balance,
        input_phone_number,
        create_button,
        cancel_button
    });

    auto renderer = Renderer(component, [&] {
        return 
            center(
                window(text("Utwórz Rachunek"), vbox(
                    vbox(
                        input_name->Render() | borderLight,
                        input_balance->Render() | borderLight,
                        input_phone_number->Render() | borderLight
                    ),
                    separator(),
                    hbox(
                        create_button->Render() | flex,
                        cancel_button->Render()
                    )
                    )
                ) | size(WIDTH, EQUAL, 60)
            );
    });

    screen.Loop(renderer);
}

std::vector<std::string> getAccountNames(const std::vector<Account>& accounts) {
    std::vector<std::string> accountNames;
    for (auto& account : accounts) {
        accountNames.push_back(account.name);
    }
    return accountNames;
}

/// @brief Wyświetla panel użytkownika
/// @param user - użytkownik
void Dashboard(User& user)
{
    auto screen = ScreenInteractive::Fullscreen();

    auto accounts = GetUserAccounts(user.id);
    auto account_names = getAccountNames(accounts);
    auto refresh_accounts = [&] {
        accounts = GetUserAccounts(user.id);
        account_names = getAccountNames(accounts);
    };
    // jeśli nie ma żadnych kont, to tworzymy je
    while (accounts.empty()) {
        CreateAccountPanel(user);
        refresh_accounts();
    }
    int selected_account_id = 0;

    FlexboxConfig flexbox_config;
    flexbox_config.justify_content = FlexboxConfig::JustifyContent::SpaceBetween;
    flexbox_config.direction = FlexboxConfig::Direction::Row;

    // function that returns a Component that displays account details
    auto account_details = [&] {
        auto account = accounts[selected_account_id];
        auto balance = std::to_string(account.balance);
        auto phone_number = std::to_string(account.phone_number);
        auto account_name = account.name;

        return flexbox({
            vbox({
                text("Nazwa konta"),
                text(account_name),
            }),
            // whitespace
            text("  "),
            vbox({
                text("Saldo"),
                text(balance),
            }),
            text("  "),
            vbox({
                text("Numer telefonu"),
                text(phone_number),
            }),
        }, flexbox_config);
    };

    auto selected_account = accounts[selected_account_id];
    auto radiobox = Radiobox(&account_names, &selected_account_id);

    auto logout_button = Button("Wyjdź z programu", [&] {
        screen.ExitLoopClosure()();
    });
    auto create_account_button = Button("Utwórz konto", [&] {
        CreateAccountPanel(user);
        refresh_accounts();
    });

    auto component = Container::Vertical({
        create_account_button,
        logout_button,
        radiobox
    });

    // TODO twoje konta, historia transakcji, przelew, wyloguj się

    auto renderer = Renderer(component, [&] {
        return 
            center(
                hbox(
                    // wybór konta, tworzenie konta
                    // konto wybiera się radioboxem 
                    window(text("Twoje konta"), vbox(
                        // wyświetlanie wszystkich kont
                        radiobox->Render() | flex,
                        separator(),
                        create_account_button->Render()
                    )),

                    // TODO twoje konta, historia transakcji, przelew
                    window( text("Witaj, " + user.name + "!"),
                        vbox(
                            account_details(),
                            separator(),

                            center(text("<insert historia transakcji here>")) | flex,

                            separator(),
                            logout_button->Render()
                        )
                ) | size(WIDTH, EQUAL, 70) | size(HEIGHT, GREATER_THAN, 30)
                )
            );
    });

    screen.Loop(renderer);
}
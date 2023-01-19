#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/string.hpp>
#include <ftxui/component/component_options.hpp>  // for InputOption
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <Repositories/user.hpp>
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

    std::string first_name;
    std::string password2;
    std::string phone;

    Component input_login = Input(&login, "login");
    Component input_first_name = Input(&first_name, "imię");
    InputOption password_option;
    password_option.password = true;
    Component input_password = Input(&password, "hasło", password_option);
    Component input_password2 = Input(&password2, "powtórz hasło", password_option);

    Component registerbutton = Button("Zarejestruj się", [&] {
        if (password != password2) {
            Dialog("Hasła nie są takie same");
            return;
        }
        auto result = Register(login, password);
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
        input_login,
        input_first_name,
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
                        input_first_name->Render() | borderLight,
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

/// @brief Wyświetla panel użytkownika
/// @param user - użytkownik
void Dashboard(User& user)
{
    auto screen = ScreenInteractive::Fullscreen();

    auto logout_button = Button("Zamknij", [&] {
        screen.ExitLoopClosure()();
    });

    auto component = Container::Vertical({
        logout_button
    });

    // TODO twoje konta, historia transakcji, przelew, wyloguj się

    auto renderer = Renderer(component, [&] {
        return 
            center(
                window(text("Witaj, " + user.name + "!"), vbox(
                    vbox(
                    ),
                    separator(),
                    logout_button->Render()
                    )
                ) | size(WIDTH, EQUAL, 60)
            );
    });

    screen.Loop(renderer);
}
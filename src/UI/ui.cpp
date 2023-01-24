#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/string.hpp>
#include <ftxui/component/component_options.hpp>  // for InputOption
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <Repositories/user.hpp>
#include <Repositories/repos.hpp>
#include "ui.hpp"
#include "transferui.hpp"

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
        if (phone_number.empty()) {
            Dialog("Numer telefonu nie może być pusty");
            return;
        }

        double balance_value;

        if (balance.empty()) {
            balance_value = 0;
        } else {
            try {
                balance_value = std::stod(balance);
            } catch (const std::invalid_argument& e) {
                Dialog("Saldo musi być liczbą");
                return;
            } catch (const std::out_of_range& e) {
                Dialog("Saldo jest zbyt duże");
                return;
            }
        }

        // bezpieczne rzutowanie na double balance i phone_number
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

        //auto result = CreateUserAccount(user.id, name, phone_number_value);
        auto result = Account::parseAccountValues(user.id, name, phone_number_value);

        switch (result) {
            case ParsingAccountValuesStatus::SUCCESS: {
                Account acc {user.id, name, balance_value, phone_number_value};
                Repo<Account>::Insert(acc);
                Dialog("Utworzono konto!");
                screen.ExitLoopClosure()();
                break;
            }
            case ParsingAccountValuesStatus::FIELDS_EMPTY:
                Dialog("Pola nie mogą być puste");
                break;
            case ParsingAccountValuesStatus::PHONE_NUMBER_EXISTS:
                Dialog("Konto z podanym numerem telefonu już istnieje");
                break;
            case ParsingAccountValuesStatus::BALANCE_OUT_OF_RANGE:
                Dialog("Saldo jest zbyt duże");
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

void CreateCreditCardPanel(Account& account)
{
    auto screen = ScreenInteractive::Fullscreen();

    std::string name;
    std::string pin;

    Component input_name = Input(&name, "Nazwa karty");
    Component input_pin = Input(&pin, "PIN");

    Component create_button = Button("Utwórz", [&] {
        if (name.empty()) {
            Dialog("Nazwa karty nie może być pusta");
            return;
        }

        if (pin.empty()) {
            Dialog("PIN nie może być pusty");
            return;
        }

        if (pin.length() != 4) {
            Dialog("PIN musi mieć 4 cyfry");
            return;
        }

        // bezpieczne na int pin
        int pin_value;
        try {
            pin_value = std::stoi(pin);
        } catch (const std::invalid_argument& e) {
            Dialog("PIN musi być liczbą");
            return;
        } catch (const std::out_of_range& e) {
            Dialog("PIN jest zbyt duży");
            return;
        }

        auto result = CreateCreditCard(account.id, name, pin_value);
        switch (result) {
            case CreateCreditCardResult::SUCCESS:
                Dialog("Utworzono kartę! Jej szczegóły znajdziesz w zakładce 'Karty Kredytowe'");
                screen.ExitLoopClosure()();
                break;
            case CreateCreditCardResult::INTERNAL_ERROR:
                Dialog("Błąd wewnętrzny");
                break;
            case CreateCreditCardResult::FIELDS_EMPTY:
                Dialog("Pola nie mogą być puste");
                break;
        }
    });

    Component cancel_button = Button("Anuluj", [&] {
        screen.ExitLoopClosure()();
    });

    auto component = Container::Vertical({
        input_name,
        input_pin,
        create_button,
        cancel_button
    });

    auto renderer = Renderer(component, [&] {
        return 
            center(
                window(text("Utwórz Kartę Kredytową"), vbox(
                    vbox(
                        input_name->Render() | borderLight,
                        input_pin->Render() | borderLight
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

std::vector<std::string> getCreditCardNames(const std::vector<CreditCard>& credit_cards) {
    std::vector<std::string> creditCardNames;
    for (auto& credit_card : credit_cards) {
        creditCardNames.push_back(credit_card.name);
    }
    return creditCardNames;
}

// TransactionData(uint32_t sender_id, uint32_t recipent_id, double amount)
Elements TransactionHistory(const std::vector<TransactionData>& transactions, int account_id)
{
    std::vector<Element> transaction_components;

    for (auto& transaction : transactions) {
        auto sender_account = Database::getStorage() -> get<Account>(transaction.sender_account_id);
        auto sender_user = Database::getStorage() -> get<User>(sender_account.user_id);
        auto recipient_account = Database::getStorage() -> get<Account>(transaction.recipient_account_id);
        auto recipient_user = Database::getStorage() -> get<User>(recipient_account.user_id);

        // show either as sent or received
        // when sent, display ammount in red
        // when received, display ammount in green
        if (transaction.sender_account_id == account_id) {
            transaction_components.push_back(
                text(" Wysłano " + Utils::double_to_string(transaction.amount) + " zł do " + recipient_user.name + " (" + recipient_account.name + ") ") | color(Color::Red)
            );
        } else {
            transaction_components.push_back(
                text(" Otrzymano " + Utils::double_to_string(transaction.amount) + " zł od " + sender_user.name + " (" + sender_account.name + ") ") | color(Color::Green)
            );
        }

        transaction_components.push_back(separator());
    }

    if (transaction_components.empty()) {
        transaction_components.push_back(text("Nie masz jeszcze żadnych transakcji!"));
    }

    return transaction_components;
}

/// @brief Wyświetla panel użytkownika
/// @param user - użytkownik
void Dashboard(User& user)
{

    auto screen = ScreenInteractive::Fullscreen();

    int selected_account_id = 0;
    auto accounts = GetUserAccounts(user.id);
    auto account_names = getAccountNames(accounts);

    // potrzebne dla poniższego while loopa
    auto refresh_data_accounts_only = [&] {
        accounts = GetUserAccounts(user.id);
        account_names = getAccountNames(accounts);
    };

    // jeśli nie ma żadnych kont, to tworzymy je
    while (accounts.empty()) {
        CreateAccountPanel(user);
        refresh_data_accounts_only();
    }

    int selected_credit_card_id = 0;
    auto credit_cards = GetCreditCardsByAccountId(accounts[selected_account_id].id);
    auto credit_card_names = getCreditCardNames(credit_cards);

    auto refresh_data_cards_only = [&] {
        credit_cards = GetCreditCardsByAccountId(accounts[selected_account_id].id);
        credit_card_names = getCreditCardNames(credit_cards);
    };

    auto transactions = GetTransactionsByAccountId(accounts[selected_account_id].id);
    auto transaction_elements = TransactionHistory(transactions, accounts[selected_account_id].id);

    auto refresh_data_transactions_only = [&] {
        transactions = GetTransactionsByAccountId(accounts[selected_account_id].id);
        transaction_elements = TransactionHistory(transactions, accounts[selected_account_id].id);
    };

    auto refresh_data = [&] {
        refresh_data_transactions_only();
        refresh_data_accounts_only();
        refresh_data_cards_only();
    };

    FlexboxConfig flexbox_config;
    flexbox_config.justify_content = FlexboxConfig::JustifyContent::SpaceBetween;
    flexbox_config.align_items = FlexboxConfig::AlignItems::Stretch;
    flexbox_config.direction = FlexboxConfig::Direction::Row;

    // function that returns a Component that displays account details
    auto account_details = [&] {
        auto account = accounts[selected_account_id];
        auto balance = Utils::double_to_string(account.balance);
        auto phone_number = std::to_string(account.phone_number);
        auto account_name = account.name;

        // text(" ") - dodaje puste miejsce
        return flexbox({
            text(" "),                    
            vbox({
                text("Nazwa konta"),
                text(account_name) | bold,
            }) | flex,
            text(" "),            
            separator(),
            text(" "),            
            vbox({
                text("Saldo"),
                text(balance) | bold,
            }) | flex,
            text(" "),            
            separator(),
            text(" "),            
            vbox({
                text("Numer telefonu"),
                text(phone_number) | bold,
            }) | flex,
            text(" "),                        
        }, flexbox_config);
    };

    auto radiobox = Radiobox(&account_names, &selected_account_id);

    auto make_transfer_button = Button(" ⇄ Wykonaj Transfer ", [&] {
        // TODO segfault może być bo selected_account jest z array?
        TransferPanel(accounts[selected_account_id]);
        refresh_data();
    });
    auto logout_button = Button(" ✕ Wyjdź z programu ", [&] {
        screen.ExitLoopClosure()();
    });
    auto create_account_button = Button(" + Nowy Rachunek ", [&] {
        CreateAccountPanel(user);
        refresh_data();
    });

    auto create_credit_card_button = Button(" + Nowa Karta Kredytowa ", [&] {
        CreateCreditCardPanel(accounts[selected_account_id]);
        refresh_data();
    });

    auto credit_card_radiobox = Radiobox(&credit_card_names, &selected_credit_card_id);

    auto credit_card_component = Container::Vertical({
        credit_card_radiobox,
        create_credit_card_button
    });

    // for now it's just a placeholder
    auto creditCardRenderer = Renderer(credit_card_component, [&] {
        refresh_data_cards_only();

        if(credit_cards.empty()){
            return 
                center(
                    vbox(
                        text("Karty Kredytowe:") | bold,
                        vbox(text("Brak kart kredytowych!")) | flex,
                        separator(),
                        create_credit_card_button->Render()
                    )
                );
        }

        return 
            center(
                vbox(
                    text("Karty Kredytowe:") | bold,
                    // radio box
                    credit_card_radiobox->Render(),
                    separator(),
                    hbox(vbox(
                        // display the credit card details
                        text("Nazwa karty: ") | bold,
                        text(credit_cards[selected_credit_card_id].name),
                        text(""),
                        text("Numer karty: ") | bold,
                        text(std::to_string(credit_cards[selected_credit_card_id].number)),
                        text(""),
                        text("CVV: ") | bold,
                        text(std::to_string(credit_cards[selected_credit_card_id].cvv)),
                        text(""),
                        text("Data ważności: ") | bold,
                        text(std::to_string(credit_cards[selected_credit_card_id].expiration_month) + "/" + std::to_string(credit_cards[selected_credit_card_id].expiration_year)),
                        text(""),
                        text("Pin: ") | bold,
                        text(std::to_string(credit_cards[selected_credit_card_id].pin))
                    )) | flex,
                    separator(),
                    create_credit_card_button -> Render()
                )
            );
    });

    auto historyTransactionRenderer = Renderer([&] {
        refresh_data_transactions_only();
        return 
            vbox(transaction_elements) | flex;
    });

    int left_size = 30;
    auto split = ResizableSplitLeft(creditCardRenderer, historyTransactionRenderer, &left_size);

    auto component = Container::Vertical({
        radiobox,
        split,
        create_account_button,
        make_transfer_button,
        logout_button,
    });


    // TODO twoje konta, historia transakcji, przelew, wyloguj się
    // TODO ResizableSplitLeft
    auto mainRenderer = Renderer(component, [&] {
        return 
            center(
                hbox(
                    // wybór konta, tworzenie konta
                    // konto wybiera się radioboxem 
                    window(text("Twoje Rachunki"), 
                        vbox(
                            // wyświetlanie wszystkich kont
                            hbox(
                                text(" ") | size(HEIGHT, EQUAL, 1),
                                radiobox->Render()
                            ) | flex,
                            separator(),
                            create_account_button->Render()
                        )
                    ),

                    // TODO twoje konta, historia transakcji, przelew
                    window( text("Witaj, " + user.name + "!"),
                        vbox(
                            account_details(),
                            separator(),

                            split -> Render() | flex,

                            separator(),
                            hbox(
                                make_transfer_button->Render() | flex,
                                logout_button->Render()
                            )
                        )
                ) | size(WIDTH, EQUAL, 90) | size(HEIGHT, GREATER_THAN, 40)
                )
            );
    });

    screen.Loop(mainRenderer);
}
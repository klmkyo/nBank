#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/string.hpp>
#include <ftxui/component/component_options.hpp>  // for InputOption
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <Repositories/user.hpp>
#include <Repositories/repos.hpp>
#include <Logic/payments.hpp>
#include "ui.hpp"

using namespace ftxui;

void HandleTransactionResult(const TransactionResult& result)
{
    switch(result){
        case TransactionResult::ACCOUNT_NOT_FOUND:
            Dialog("Nie znaleziono konta o podanym numerze karty!");
            break;
        case TransactionResult::WRONG_PIN:
            Dialog("Podano nieprawidłowy PIN!");
            break;
        case TransactionResult::NO_MONEY:
            Dialog("Nie masz wystarczającej ilości środków na koncie!");
            break;
        case TransactionResult::SUCCESS:
            Dialog("Przelew został wykonany pomyślnie!");
            break;
    }
}

// DirectTransfer(Account* account, double amount, int card_number)
Component DirectTransferPanel(Account& account)
{
    // TODO! to segfaultuje ponieważ stringi są dropowane pod koniec funkcji (?)
    // rust by to wyłapał
    std::string ammount;
    std::string card_number;

    Component input_ammount = Input(&ammount, "Kwota do przelania");
    Component input_card_number = Input(&card_number, "Numer karty odbiorcy");

    Component send_button = Button(L"Wyślij", [&]{
        if (ammount.empty()){
            Dialog("Kwota do przelania nie może być pusta!");
        }
        else if (card_number.empty()){
            Dialog("Numer karty odbiorcy nie może być pusty!");
        }

        // bezpieczne rzutowanie na int
        int card_number_value;
        try {
            card_number_value = std::stoi(card_number);
        } catch (std::invalid_argument& e) {
            Dialog("Numer karty odbiorcy musi być liczbą!");
        }

        int ammount_value;
        try {
            ammount_value = std::stoi(ammount);
        } catch (std::invalid_argument& e) {
            Dialog("Kwota do przelania musi być liczbą!");
        }

        TransactionResult result;
        DirectTransfer transfer(&account, ammount_value, card_number_value);
        transfer.Execute(result);

        HandleTransactionResult(result);
    });

    auto component = Container::Vertical({
        input_ammount,
        input_card_number,
        send_button
    });

    auto renderer = Renderer(component, [=] {
        return 
            vbox({
                text(L"Przelew bezpośredni"),
                // segfaultuje przez poniższe linijki
                 input_ammount->Render(),
                // input_card_number->Render(),
                // send_button->Render()
            });
    });

    return renderer;
}

// BLIKTransfer(Account* account, double amount, int phone_number) 
Component BLIKTransferPanel(Account& account, std::function<void()> exit_func)
{
    std::string ammount;
    std::string phone_number;

    Component input_ammount = Input(&ammount, "Kwota do przelania");
    Component input_phone_number = Input(&phone_number, "Numer telefonu odbiorcy");

    Component exit_button = Button(L"Anuluj", exit_func);

    Component send_button = Button(L"Wyślij", [&]{
        if (ammount.empty()){
            Dialog("Kwota do przelania nie może być pusta!");
        }
        else if (phone_number.empty()){
            Dialog("Numer telefonu odbiorcy nie może być pusty!");
        }

        // bezpieczne rzutowanie na int
        int phone_number_value;
        try {
            phone_number_value = std::stoi(phone_number);
        } catch (std::invalid_argument& e) {
            Dialog("Numer telefonu odbiorcy musi być liczbą!");
        }

        int ammount_value;
        try {
            ammount_value = std::stoi(ammount);
        } catch (std::invalid_argument& e) {
            Dialog("Kwota do przelania musi być liczbą!");
        }

        TransactionResult result;
        BLIKTransfer transfer(&account, ammount_value, phone_number_value);
        transfer.Execute(result);

        HandleTransactionResult(result);
    });

    auto component = Container::Vertical({
        input_ammount,
        input_phone_number,
        send_button,
        exit_button
    });

    auto renderer = Renderer(component, [&] {
        return 
            vbox({
                input_ammount->Render(),
                input_phone_number->Render(),
                send_button->Render(),
                exit_button->Render()
            });
    });

    return renderer;
}

// DepositTransaction(const CreditInput& input, double amount)
Component DepositPanel()
{
    std::string ammount;
    std::string card_number;
    std::string card_expiration_year;
    std::string card_expiration_month;
    std::string card_cvv;
    std::string pin;

    Component input_ammount = Input(&ammount, "Kwota do wpłaty");
    Component input_card_number = Input(&card_number, "Numer karty");
    Component input_card_expiration_year = Input(&card_expiration_year, "Rok ważności karty");
    Component input_card_expiration_month = Input(&card_expiration_month, "Miesiąc ważności karty");
    Component input_card_cvv = Input(&card_cvv, "CVV karty");
    Component input_pin = Input(&pin, "PIN");

    Component send_button = Button(L"Wyślij", [&]{
        if (ammount.empty()){
            Dialog("Kwota do wpłaty nie może być pusta!");
        }
        else if (card_number.empty()){
            Dialog("Numer karty nie może być pusty!");
        }
        else if (card_expiration_year.empty()){
            Dialog("Rok ważności karty nie może być pusty!");
        }
        else if (card_expiration_month.empty()){
            Dialog("Miesiąc ważności karty nie może być pusty!");
        }
        else if (card_cvv.empty()){
            Dialog("CVV karty nie może być pusty!");
        }
        else if (pin.empty()){
            Dialog("PIN nie może być pusty!");
        }

        // bezpieczne rzutowanie na int
        int card_number_value;
        try {
            card_number_value = std::stoi(card_number);
        } catch (std::invalid_argument& e) {
            Dialog("Numer karty musi być liczbą!");
        }

        int card_expiration_year_value;
        try {
            card_expiration_year_value = std::stoi(card_expiration_year);
        } catch (std::invalid_argument& e) {
            Dialog("Rok ważności karty musi być liczbą!");
        }

        int card_expiration_month_value;
        try {
            card_expiration_month_value = std::stoi(card_expiration_month);
        } catch (std::invalid_argument& e) {
            Dialog("Miesiąc ważności karty musi być liczbą!");
        }

        int card_cvv_value;
        try {
            card_cvv_value = std::stoi(card_cvv);
        } catch (std::invalid_argument& e) {
            Dialog("CVV karty musi być liczbą!");
        }

        int pin_value;
        try {
            pin_value = std::stoi(pin);
        } catch (std::invalid_argument& e) {
            Dialog("PIN musi być liczbą!");
        }

        double ammount_value;
        try {
            ammount_value = std::stod(ammount);
        } catch (std::invalid_argument& e) {
            Dialog("Kwota do wpłaty musi być liczbą!");
        }

        TransactionResult result;
        // CreditInput(number, cvv, exp_month, exp_year, pin)
        CreditInput input(card_number_value, card_cvv_value, card_expiration_month_value, card_expiration_year_value, pin_value);
        DepositTransaction transaction(input, ammount_value);
        transaction.Execute(result);

        HandleTransactionResult(result);
    });

    auto component = Container::Vertical({
        input_ammount,
        input_card_number,
        input_card_expiration_year,
        input_card_expiration_month,
        input_card_cvv,
        input_pin,
        send_button
    });

    auto renderer = Renderer(component, [&] {
        return 
            vbox({
                input_ammount->Render(),
                input_card_number->Render(),
                input_card_expiration_year->Render(),
                input_card_expiration_month->Render(),
                input_card_cvv->Render(),
                input_pin->Render(),
                send_button->Render()
            });
    });

    return renderer;
}

// Transfer type selector
// radiobox with the transfer types on the left, and the selected transfer panel on the right
void TransferPanel(Account& account)
{
    auto screen = ScreenInteractive::Fullscreen();

    // pairs of transfer string and transfer panel
    std::vector<std::string> transfer_names = {
        "Transfer bezpośredni",
        "BLIK",
        "Wpłata gotówki"
    };
    Component transfer_components[] = {
        DirectTransferPanel(account),
        BLIKTransferPanel(account, [&]{screen.ExitLoopClosure()();}),
        DepositPanel()
    };
    int selected_transfer_index = 0;

    Component transfer_type_selector = Radiobox(&transfer_names, &selected_transfer_index);

    auto component = Container::Horizontal({
        transfer_type_selector,
        transfer_components[selected_transfer_index]
    });

    auto renderer = Renderer(component, [&] {
        return center(
            window(text(L"Przelew"),
                hbox({
                    transfer_type_selector->Render(),
                    transfer_components[selected_transfer_index]->Render()
                })
            )
        );
    });

    screen.Loop(renderer);
}
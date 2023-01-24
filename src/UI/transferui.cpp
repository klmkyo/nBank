#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/string.hpp>
#include <ftxui/component/component_options.hpp>  // for InputOption
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include "Logic/payments.hpp"
#include "UI/ui.hpp"

#define NO_PIN_LIMIT 10.0

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
void DirectTransferPanel(Account& account)
{
    auto screen = ScreenInteractive::Fullscreen();

    std::string ammount;
    std::string card_number;

    Component input_ammount = Input(&ammount, "Kwota do przelania");
    Component input_card_number = Input(&card_number, "Numer karty odbiorcy");

    Component send_button = Button(L"Wyślij", [&]{
        if (ammount.empty()){
            Dialog("Kwota do przelania nie może być pusta!");
            return;
        }
        else if (card_number.empty()){
            Dialog("Numer karty odbiorcy nie może być pusty!");
            return;
        }

        // bezpieczne rzutowanie na int
        int card_number_value;
        try {
            card_number_value = std::stoi(card_number);
        } catch (std::invalid_argument& e) {
            Dialog("Numer karty odbiorcy musi być liczbą!");
            return;
        }

        int ammount_value;
        try {
            ammount_value = std::stoi(ammount);
        } catch (std::invalid_argument& e) {
            Dialog("Kwota do przelania musi być liczbą!");
            return;
        }

        TransactionResult result;
        DirectTransfer transfer(account, ammount_value, card_number_value);
        transfer.Execute(result);

        HandleTransactionResult(result);
    });

    auto exit_button = Button(L"Anuluj", [&]{
        screen.ExitLoopClosure()();
    });

    auto component = Container::Vertical({
        input_ammount,
        input_card_number,
        send_button,
        exit_button
    });

    auto renderer = Renderer(component, [&] {
        return 
            center(
                window(text(L"Przelew bezpośredni"), vbox({
                    input_ammount->Render() | borderLight,
                    input_card_number->Render() | borderLight,
                    hbox({
                        exit_button->Render(),
                        send_button->Render() | flex
                    })
                })) | size(WIDTH, EQUAL, 50)
            );
    });

    screen.Loop(renderer);
}

// WithdrawTransaction(const CreditInput& input, double amount)
// CreditInput(number, cvv, exp_month, exp_year, pin)
void WithdrawPanel()
{
    auto screen = ScreenInteractive::Fullscreen();

    std::string ammount;
    std::string card_number;
    std::string cvv;
    std::string exp_month;
    std::string exp_year;
    std::string pin;

    Component input_ammount = Input(&ammount, "Kwota do wypłaty");
    Component input_card_number = Input(&card_number, "Numer karty");
    Component input_cvv = Input(&cvv, "CVV");
    Component input_exp_month = Input(&exp_month, "Miesiąc ważności karty");
    Component input_exp_year = Input(&exp_year, "Rok ważności karty");
    Component input_pin = Input(&pin, "PIN");

    Component send_button = Button(L"Wyślij", [&]{
        if (ammount.empty()){
            Dialog("Kwota do wypłaty nie może być pusta!");
            return;
        }
        else if (card_number.empty()){
            Dialog("Numer karty nie może być pusty!");
            return;
        }
        else if (cvv.empty()){
            Dialog("CVV nie może być pusty!");
            return;
        }
        else if (exp_month.empty()){
            Dialog("Miesiąc ważności karty nie może być pusty!");
            return;
        }
        else if (exp_year.empty()){
            Dialog("Rok ważności karty nie może być pusty!");
            return;
        }
        else if (pin.empty()){
            Dialog("PIN nie może być pusty!");
            return;
        }

        // bezpieczne rzutowanie na int
        int card_number_value;
        try {
            card_number_value = std::stoi(card_number);
        } catch (std::invalid_argument& e) {
            Dialog("Numer karty musi być liczbą!");
            return;
        }

        int cvv_value;
        try {
            cvv_value = std::stoi(cvv);
        } catch (std::invalid_argument& e) {
            Dialog("CVV musi być liczbą!");
            return;
        }

        int exp_month_value;
        try {
            exp_month_value = std::stoi(exp_month);
        } catch (std::invalid_argument& e) {
            Dialog("Miesiąc ważności karty musi być liczbą!");
            return;
        }

        int exp_year_value;
        try {
            exp_year_value = std::stoi(exp_year);
        } catch (std::invalid_argument& e) {
            Dialog("Rok ważności karty musi być liczbą!");
            return;
        }

        int ammount_value;
        try {
            ammount_value = std::stoi(ammount);
        } catch (std::invalid_argument& e) {
            Dialog("Kwota do wypłaty musi być liczbą!");
            return;
        }

        int pin_value;
        try {
            pin_value = std::stoi(pin);
        } catch (std::invalid_argument& e) {
            Dialog("PIN musi być liczbą!");
            return;
        }

        CreditInput input(card_number_value, cvv_value, exp_month_value, exp_year_value, pin_value);
        TransactionResult result;
        WithdrawTransaction transaction(input, ammount_value);
        transaction.Execute(result);

        HandleTransactionResult(result);
    });

    auto exit_button = Button(L"Anuluj", [&]{
        screen.ExitLoopClosure()();
    });

    auto component = Container::Vertical({
        input_ammount,
        input_card_number,
        input_cvv,
        input_exp_month,
        input_exp_year,
        input_pin,
        send_button,
        exit_button
    });

    auto renderer = Renderer(component, [&] {
        return 
            center(
                window(text(L"Wypłata gotówki"), vbox({
                    input_ammount->Render() | borderLight,
                    input_card_number->Render() | borderLight,
                    input_cvv->Render() | borderLight,
                    input_exp_month->Render() | borderLight,
                    input_exp_year->Render() | borderLight,
                    input_pin->Render() | borderLight,
                    hbox({
                        exit_button->Render(),
                        send_button->Render() | flex
                    })
                })) | size(WIDTH, EQUAL, 50)
            );
    });

    screen.Loop(renderer);
}

// BLIKTransfer(Account* account, double amount, int phone_number) 
void BLIKTransferPanel(Account& account)
{
    auto screen = ScreenInteractive::Fullscreen();

    std::string ammount;
    std::string phone_number;

    Component input_ammount = Input(&ammount, "Kwota do przelania");
    Component input_phone_number = Input(&phone_number, "Numer telefonu odbiorcy");

    Component send_button = Button(L"Wyślij", [&]{
        if (ammount.empty()){
            Dialog("Kwota do przelania nie może być pusta!");
            return;
        }
        else if (phone_number.empty()){
            Dialog("Numer telefonu odbiorcy nie może być pusty!");
            return;
        }

        // bezpieczne rzutowanie na int
        int phone_number_value;
        try {
            phone_number_value = std::stoi(phone_number);
        } catch (std::invalid_argument& e) {
            Dialog("Numer telefonu odbiorcy musi być liczbą!");
            return;
        }

        int ammount_value;
        try {
            ammount_value = std::stoi(ammount);
        } catch (std::invalid_argument& e) {
            Dialog("Kwota do przelania musi być liczbą!");
            return;
        }

        TransactionResult result;
        BLIKTransfer transfer(account, ammount_value, phone_number_value);
        transfer.Execute(result);

        HandleTransactionResult(result);
    });

    auto exit_button = Button(L"Anuluj", [&]{
        screen.ExitLoopClosure()();
    });

    auto component = Container::Vertical({
        input_ammount,
        input_phone_number,
        send_button,
        exit_button
    });

    auto renderer = Renderer(component, [&] {
        return 
            center(
                window(text(L"Przelew BLIK"), vbox({
                    input_ammount->Render() | borderLight,
                    input_phone_number->Render() | borderLight,
                    hbox({
                        exit_button->Render(),
                        send_button->Render() | flex
                    })
                })) | size(WIDTH, EQUAL, 50)
            );
    });

    screen.Loop(renderer);
}

// CardTransaction(const CreditInput& input, double amount)
// CreditInput(number, cvv, exp_month, exp_year, pin;
void CardTransactionPanel()
{
    auto screen = ScreenInteractive::Fullscreen();

    std::string ammount;
    std::string card_number;
    std::string card_expiration_year;
    std::string card_expiration_month;
    std::string card_cvv;
    std::string pin;

    Component input_ammount = Input(&ammount, "Kwota do przelania");
    Component input_card_number = Input(&card_number, "Numer karty");
    Component input_card_expiration_year = Input(&card_expiration_year, "Rok ważności karty");
    Component input_card_expiration_month = Input(&card_expiration_month, "Miesiąc ważności karty");
    Component input_card_cvv = Input(&card_cvv, "CVV karty");
    Component input_pin = Input(&pin, "PIN (wymagany powyżej " + std::to_string(NO_PIN_LIMIT) + ")");

    Component send_button = Button(L"Wyślij", [&]{
        if (ammount.empty()){
            Dialog("Kwota do przelania nie może być pusta!");
            return;
        }
        else if (card_number.empty()){
            Dialog("Numer karty nie może być pusty!");
            return;
        }
        else if (card_expiration_year.empty()){
            Dialog("Rok ważności karty nie może być pusty!");
            return;
        }
        else if (card_expiration_month.empty()){
            Dialog("Miesiąc ważności karty nie może być pusty!");
            return;
        }
        else if (card_cvv.empty()){
            Dialog("CVV karty nie może być pusty!");
            return;
        }

        // bezpieczne rzutowanie na int
        int card_number_value;
        try {
            card_number_value = std::stoi(card_number);
        } catch (std::invalid_argument& e) {
            Dialog("Numer karty musi być liczbą!");
            return;
        }

        int cvv_value;
        try {
            cvv_value = std::stoi(card_cvv);
        } catch (std::invalid_argument& e) {
            Dialog("CVV karty musi być liczbą!");
            return;
        }

        int exp_month_value;
        try {
            exp_month_value = std::stoi(card_expiration_month);
        } catch (std::invalid_argument& e) {
            Dialog("Miesiąc ważności karty musi być liczbą!");
            return;
        }

        int exp_year_value;
        try {
            exp_year_value = std::stoi(card_expiration_year);
        } catch (std::invalid_argument& e) {
            Dialog("Rok ważności karty musi być liczbą!");
            return;
        }

        int ammount_value;
        try {
            ammount_value = std::stoi(ammount);
        } catch (std::invalid_argument& e) {
            Dialog("Kwota do przelania musi być liczbą!");
            return;
        }

        if (pin.empty() && ammount_value >= NO_PIN_LIMIT){
            Dialog("PIN nie może być pusty!");
            return;
        }

        int pin_value;
        try {
            pin_value = std::stoi(pin);
        } catch (std::invalid_argument& e) {
            if (ammount_value >= NO_PIN_LIMIT){
                Dialog("PIN musi być liczbą!");
                return;
            } else {
                pin_value = 0;
            }
        }

        TransactionResult result;
        // CardTransaction(const CreditInput& input, double amount)
        // CreditInput(int number, int cvv, int exp_month, int exp_year, int pin)
        CardTransaction transaction(CreditInput(card_number_value, cvv_value, exp_month_value, exp_year_value, pin_value), ammount_value);

        transaction.Execute(result);

        HandleTransactionResult(result);
    });

    auto exit_button = Button(L"Anuluj", [&]{
        screen.ExitLoopClosure()();
    });

    auto component = Container::Vertical({
        input_ammount,
        input_card_number,
        input_card_expiration_year,
        input_card_expiration_month,
        input_card_cvv,
        input_pin,
        send_button,
        exit_button
    });

    auto renderer = Renderer(component, [&] {
        return 
            center(
                window(text("Płatność kartą kredytową"), vbox({
                    input_ammount->Render() | borderLight,
                    input_card_number->Render() | borderLight,
                    input_card_expiration_year->Render() | borderLight,
                    input_card_expiration_month->Render() | borderLight,
                    input_card_cvv->Render() | borderLight,
                    input_pin->Render() | borderLight,
                    hbox({
                        exit_button->Render(),
                        send_button->Render() | flex
                    })
                })) | size(WIDTH, EQUAL, 50)
            );
    });

    screen.Loop(renderer);
}


// DepositTransaction(const CreditInput& input, double amount)
void DepositPanel()
{
    auto screen = ScreenInteractive::Fullscreen();

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
            return;
        }
        else if (card_number.empty()){
            Dialog("Numer karty nie może być pusty!");
            return;
        }
        else if (card_expiration_year.empty()){
            Dialog("Rok ważności karty nie może być pusty!");
            return;
        }
        else if (card_expiration_month.empty()){
            Dialog("Miesiąc ważności karty nie może być pusty!");
            return;
        }
        else if (card_cvv.empty()){
            Dialog("CVV karty nie może być pusty!");
            return;
        }
        else if (pin.empty()){
            Dialog("PIN nie może być pusty!");
            return;
        }

        // bezpieczne rzutowanie na int
        int card_number_value;
        try {
            card_number_value = std::stoi(card_number);
        } catch (std::invalid_argument& e) {
            Dialog("Numer karty musi być liczbą!");
            return;
        }

        int card_expiration_year_value;
        try {
            card_expiration_year_value = std::stoi(card_expiration_year);
        } catch (std::invalid_argument& e) {
            Dialog("Rok ważności karty musi być liczbą!");
            return;
        }

        int card_expiration_month_value;
        try {
            card_expiration_month_value = std::stoi(card_expiration_month);
        } catch (std::invalid_argument& e) {
            Dialog("Miesiąc ważności karty musi być liczbą!");
            return;
        }

        int card_cvv_value;
        try {
            card_cvv_value = std::stoi(card_cvv);
        } catch (std::invalid_argument& e) {
            Dialog("CVV karty musi być liczbą!");
            return;
        }

        int pin_value;
        try {
            pin_value = std::stoi(pin);
        } catch (std::invalid_argument& e) {
            Dialog("PIN musi być liczbą!");
            return;
        }

        double ammount_value;
        try {
            ammount_value = std::stod(ammount);
        } catch (std::invalid_argument& e) {
            Dialog("Kwota do wpłaty musi być liczbą!");
            return;
        }

        TransactionResult result;
        // CreditInput(number, cvv, exp_month, exp_year, pin)
        CreditInput input(card_number_value, card_cvv_value, card_expiration_month_value, card_expiration_year_value, pin_value);
        DepositTransaction transaction(input, ammount_value);
        transaction.Execute(result);

        HandleTransactionResult(result);
    });

    auto exit_button = Button(L"Anuluj", [&]{
        screen.ExitLoopClosure()();
    });

    auto component = Container::Vertical({
        input_ammount,
        input_card_number,
        input_card_expiration_year,
        input_card_expiration_month,
        input_card_cvv,
        input_pin,
        send_button,
        exit_button
    });

    auto renderer = Renderer(component, [&] {
        return 
            center(
                window(text(L"Wpłata"), vbox({
                    input_ammount->Render() | borderLight,
                    input_card_number->Render() | borderLight,
                    input_card_expiration_year->Render() | borderLight,
                    input_card_expiration_month->Render() | borderLight,
                    input_card_cvv->Render() | borderLight,
                    input_pin->Render() | borderLight,
                    hbox({
                        exit_button->Render(),
                        send_button->Render() | flex
                    })
                })) | size(WIDTH, EQUAL, 50)
            );
    });

    screen.Loop(renderer);
}

// Transfer type selector
void TransferPanel(Account& account)
{
    auto screen = ScreenInteractive::Fullscreen();

    auto direct_transfer_button = Button(L"Przelew bezpośredni", [&]{
        DirectTransferPanel(account);
    });

    auto blik_transfer_button = Button(L"Przelew BLIK", [&]{
        BLIKTransferPanel(account);
    });

    auto card_transfer_button = Button(L"Płatność kartą", [&]{
        CardTransactionPanel();
    });

    auto withdraw_transfer_button = Button(L"Wypłata", [&]{
        WithdrawPanel();
    });

    auto deposit_transfer_button = Button(L"Wpłata", [&]{
        DepositPanel();
    });

    Component exit_button = Button("Powrót", [&]{
        screen.ExitLoopClosure()();
    });

    auto component = Container::Horizontal({
        direct_transfer_button,
        blik_transfer_button,
        card_transfer_button,
        withdraw_transfer_button,
        deposit_transfer_button,
        exit_button
    });


    auto renderer = Renderer(component, [&] {
        return center(
            window(text(L"Przelew"),
                vbox({
                    direct_transfer_button->Render(),
                    blik_transfer_button->Render(),
                    card_transfer_button->Render(),
                    withdraw_transfer_button->Render(),
                    deposit_transfer_button->Render(),
                    separator(),
                    exit_button->Render()
                })
            )
        );
    });

    screen.Loop(renderer);
}
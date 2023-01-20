#pragma once
#include <ftxui/component/component.hpp>
#include <Logic/payments.hpp>

void HandleTransactionResult(const TransactionResult& result);
ftxui::Component DirectTransferPanel(Account& account);
ftxui::Component BLIKTransferPanel(Account& account, std::function<void()> exit_func);
ftxui::Component DepositPanel();
ftxui::Component TransferPanel(Account& account);
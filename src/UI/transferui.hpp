#pragma once
#include <ftxui/component/component.hpp>
#include <Logic/payments.hpp>
// they need to be void, they was set to ftxui::Component which is typename for shared_ptr, even that in transferui.cpp they was set to void something overrided them 
void HandleTransactionResult(const TransactionResult& result);
void DirectTransferPanel(Account& account);
void BLIKTransferPanel(Account& account);
void DepositPanel();
void TransferPanel(Account& account);
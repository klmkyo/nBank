#pragma once
#include "Database/User.hpp"
#include "UI/transferui.hpp"

void LoginScreen(User& user);
void RegisterScreen(std::string login, std::string password);
void Dashboard(User& user);
void Dialog(const std::string& message, uint width = 60);
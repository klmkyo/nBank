#pragma once
#include "Repositories/user.hpp"

void LoginScreen(User& user);
void RegisterScreen(std::string login, std::string password);
void Dashboard(User& user);
void Dialog(const std::string& message, uint width = 60);
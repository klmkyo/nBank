#pragma once
#include <Repositories/user.hpp>

void LoginScreen(User& user);
void RegisterScreen();
void Dashboard(User& user);
void Dialog(const std::string& message);
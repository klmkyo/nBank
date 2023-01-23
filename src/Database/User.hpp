#pragma once
#include <string>
#include <iostream>

class User {
public:
  int id;

  std::string name;
  std::string login;
  std::string password_hash;
  std::string password_salt;

  friend std::ostream& operator<<(std::ostream& os, const User& u) {
    return os << "User(id=" << u.id << ", name=" << u.name << " login=" << u.login << " password_hash=" << u.password_hash << " password_salt=" << u.password_salt << ")";
  }
};
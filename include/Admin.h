#pragma once
#include <string>
#include <iostream>

using namespace std;

class Admin
{
private:
    std::string username;
    std::string password;

public:
    Admin(const std::string &user, const std::string &pass)
        : username(user), password(pass) {}

    bool login(const std::string &user, const std::string &pass) const
    {
        return (user == username && pass == password);
    }
};

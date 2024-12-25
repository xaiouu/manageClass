#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class Admin
{
private:
    struct UserAccount
    {
        string username;
        string password;
    };

    vector<UserAccount> accounts;
    const string ACCOUNTS_FILE = "../data/accounts.txt";

public:
    Admin()
    {
        loadAccounts();
    }

    bool login(const string &user, const string &pass) const
    {
        for (const auto &account : accounts)
        {
            if (account.username == user && account.password == pass)
            {
                return true;
            }
        }
        return false;
    }

    bool registerAccount(const string &user, const string &pass)
    {
        // 检查用户名是否已存在
        for (const auto &account : accounts)
        {
            if (account.username == user)
            {
                return false;
            }
        }

        accounts.push_back({user, pass});
        saveAccounts();
        return true;
    }

    bool changePassword(const string &user, const string &oldPass, const string &newPass)
    {
        for (auto &account : accounts)
        {
            if (account.username == user && account.password == oldPass)
            {
                account.password = newPass;
                saveAccounts();
                return true;
            }
        }
        return false;
    }

private:
    void loadAccounts()
    {
        ifstream file(ACCOUNTS_FILE);
        if (!file.is_open())
        {
            // 如果文件不存在，创建默认管理员账户
            accounts.push_back({"admin", "123456"});
            saveAccounts();
            return;
        }

        string line;
        while (getline(file, line))
        {
            istringstream iss(line);
            string username, password;
            if (iss >> username >> password)
            {
                accounts.push_back({username, password});
            }
        }
    }

    void saveAccounts() const
    {
        ofstream file(ACCOUNTS_FILE);
        for (const auto &account : accounts)
        {
            file << account.username << " " << account.password << "\n";
        }
    }
};

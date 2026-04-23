#pragma once
#include "../models/Account.hpp"
#include "../models/AccountException.hpp"
#include <vector>
#include <string>
#include <algorithm>

class AccountManager
{
private:
    std::vector<Account> accounts;
public:
    AccountManager(std::vector<Account> accounts) : accounts(accounts) {}
    void addAccount(Account account) {
        accounts.push_back(account);
    }
    Account* getAccount(std::string name, std::string password) {
        auto target = std::find_if(accounts.begin(), accounts.end(),
            [name](const Account& current) {
                return current.getName() == name;
            }
        );
        if (target == accounts.end()) {
            throw AccountNotFoundException();
        }
        if (!(*target).isPasswordMatch(password)) {
            throw WrongPasswordException();
        }
        return &(*target);
    }
    std::vector<Account*> getTopNAccount(int N) {
        if (N < 1) {
            throw std::invalid_argument("N < 1");
        }
        int count = accounts.size();
        if (count > N) {
            count = N;
        }
        std::sort(accounts.begin(), accounts.end(),
            [](const Account& a, const Account& b) {
                if (a.getScore() == b.getScore()) {
                    return a.getName() < b.getName();
                }
                return a.getScore() < b.getScore();
            }
        );
        std::vector<Account*> result;
        for (size_t i = 0; i < count; i++)
        {
            result.push_back(&accounts[i]);
        }
        return result;
    }
};

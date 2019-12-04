//
// Created by ida on 29.10.19.
//
#include <thread>

#include "Account.h"

using namespace std;

namespace account {

    int Account::get_balance() {
        m.lock();
        int res = balance;
        m.unlock();
        return res;
    }

    void Account::deposit(int amount) {
        std::lock_guard<std::mutex> guard(m);
        balance += amount;
    }

    bool Account::withdraw(int amount) {
        std::unique_lock<std::mutex> lock(m, std::defer_lock);
        if (balance >= amount) {
            this_thread::yield();
            balance -= amount;
            return true;
        }
        return false;
    }
}
//
// Created by ida on 29.10.19.
//

#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <thread>
#include <mutex>

namespace account {

    class Account {
    public:
        int get_balance();

        void deposit(int amount);

        bool withdraw(int amount);

    private:
        int balance{0};
        std::mutex m;
    };
}

#endif //ACCOUNT_H

//
// Created by ida on 29.10.19.
//

#ifndef DEPOSITER_H
#define DEPOSITER_H

#include <chrono>
#include <thread>

namespace account {

    class Depositer {
    public:
        Depositer(Account& account) : account{account} {};
        void operator() (){
            for (int i{}; i < 5; i++) {
                using namespace std::chrono_literals;
                std::this_thread::sleep_for(100ms * i);
                account.deposit(1);
            }
        }
    private:
        Account& account;
    };

}


#endif //DEPOSITER_H

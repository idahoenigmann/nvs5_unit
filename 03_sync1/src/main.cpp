//
// Created by ida on 10.09.19.
//

#include <iostream>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <csignal>
#include <sys/wait.h>
#include <random>
#include <iomanip>

#include "Account.h"
#include "Depositer.h"

using namespace std;
using namespace account;

int main() {
    Account account{};
    /*
    // Punkt 1
    cout << account.get_balance() << endl;
    account.deposit(10);
    cout << account.get_balance() << endl;
    account.withdraw(5);
    cout << account.get_balance() << endl;
    account.withdraw(7);
    cout << account.get_balance() << endl;
     */

    /*
    thread t1{[&](){depositer1();}};
    thread t2{[&](){depositer2();}};
     */

    Depositer depositer1{account};
    Depositer depositer2{account};
    thread t1{depositer1};
    thread t2{depositer2};

    t1.join();
    t2.join();

    cout << account.get_balance() << endl;

    return 0;
}

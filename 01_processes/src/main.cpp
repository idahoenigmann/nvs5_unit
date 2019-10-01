//
// Created by ida on 10.09.19.
//

#include <iostream>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <csignal>
#include <sys/wait.h>
#include "asdf.h"

using namespace std;

int main(int argc, char *argv[]) {
    while (true) {
        for (int i{1}; i < argc; i++) {
            cout << argv[i] << " ";
        }
        cout << endl;
        std::this_thread::sleep_for(chrono::milliseconds(500));
    }
    return 0;
}

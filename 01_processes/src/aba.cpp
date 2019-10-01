//
// Created by ida on 12.09.19.
//

#include <iostream>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <csignal>
#include <sys/wait.h>
#include <cstdlib>

using namespace std;

int main() {
    auto pid{fork()};
    if (pid != 0) {
        auto pid2{fork()};
        if (pid2 != 0) {
            std::this_thread::sleep_for(chrono::seconds(3));
            kill(pid, SIGKILL);
            int status;
            waitpid(pid, &status, 0);
            kill(pid2, SIGKILL);
            int status2;
            waitpid(pid2, &status2, 0);
        } else {
            const char* aba_letter_b{getenv("ABA_LETTER_B")};
            if (aba_letter_b) {
                execl("./go2", "go2 B", aba_letter_b, nullptr);
            } else {
                execl("./go2", "go2 B", "B", nullptr);
            }
        }
    } else {
        const char* aba_letter_a{getenv("ABA_LETTER_A")};
        if (aba_letter_a) {
            execl("./go2", "go2 A", aba_letter_a, nullptr);
        } else {
            execl("./go2", "go2 B", "A", nullptr);
        }
    }
}
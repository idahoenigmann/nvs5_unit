//
// Created by ida on 04.12.19.
//

#include <iostream>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <csignal>
#include <sys/wait.h>
#include <random>
#include <iomanip>

#include "philosopher.h"
#include "Semaphore.h"

using namespace std;

recursive_mutex out_mtx;

void println() {
    lock_guard<recursive_mutex> lock{out_mtx};
    cout << endl;
}

template<typename T, typename... Rest>
void println(const T& word, const Rest&... rest) {
    lock_guard<recursive_mutex> lock{out_mtx};
    cout << word << ' ';
    println(rest...);
}

void Philosopher::operator() (Semaphore* semaphore, bool livelock) {
    ostringstream buf;
    while (true) {
        println("Philosopher", id, "is thinking...");

        this_thread::sleep_for(1s);

        if (livelock) {
            while (true) {
                println("Philosopher", id, "attempts to get left fork");

                if (semaphore) {
                    semaphore->acquire();
                }
                left.lock();

                println("Philosopher", id, "got left fork. Now he wants the right one...");

                for (int i{}; i < 300; i++) {
                    if (right.try_lock()) {
                        break;
                    }
                    this_thread::sleep_for(10ms);
                }
                this_thread::sleep_for(100ms);
                left.unlock();
                println("Philosopher", id, "released left fork due to timeout getting the right one!");
            }
        } else {
            println("Philosopher", id, "attempts to get left fork");

            if (semaphore) {
                semaphore->acquire();
            }
            left.lock();

            println("Philosopher", id, "got left fork. Now he wants the right one...");

            this_thread::sleep_for(5s);
            right.lock();
        }

        println("Philosopher", id, "got right fork. Now he is eating...");

        this_thread::sleep_for(2s);

        println("Philosopher", id, "finished eating");

        left.unlock();
        if (semaphore) {
            semaphore->release();
        }

        println("Philosopher", id, "released left fork");

        right.unlock();

        println("Philosopher", id, "released right fork");
    }
}
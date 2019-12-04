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
#include <cstring>

#include "philosopher.h"
#include "Semaphore.h"

using namespace std;

void usage(string msg="") {

    cout << msg << endl;

    cout << "Usage: dining [OPTION]..." << endl;
    cout << "Options:" << endl;
    cout << "  --nodeadlock    enable deadlock prevention" << endl;
    cout << "  --help          display this help and exit" << endl;

    exit(1);
}

int main(int argc, char* argv[]) {
    Semaphore* semaphore = nullptr;
    bool livelock{};

    if (argc >= 2) {
        if (!strcmp(argv[1], "--nodeadlock")) {
            semaphore = new Semaphore{4};
        } else if (!strcmp(argv[1], "--livelock")) {
            livelock = true;
        } else if (!strcmp(argv[1], "--help")) {
            usage();
        } else {
            string arg = argv[1];
            usage("Error: unrecognized command line option '" + arg + "'");
        }
    }

    mutex g1;
    mutex g2;
    mutex g3;
    mutex g4;
    mutex g5;

    Philosopher p1{1, g1, g2};
    Philosopher p2{2, g2, g3};
    Philosopher p3{3, g3, g4};
    Philosopher p4{4, g4, g5};
    Philosopher p5{5, g5, g1};

    thread p1_t{p1, semaphore, livelock};
    thread p2_t{p2, semaphore, livelock};
    thread p3_t{p3, semaphore, livelock};
    thread p4_t{p4, semaphore, livelock};
    thread p5_t{p5, semaphore, livelock};

    p1_t.join();
    p2_t.join();
    p3_t.join();
    p4_t.join();
    p5_t.join();

    delete semaphore;
}

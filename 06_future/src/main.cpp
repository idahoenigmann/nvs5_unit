//
// Created by ida on 10.09.19.
//

#include <iostream>
#include <cstring>
#include <vector>
#include <future>
#include <thread>
#include <chrono>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#pragma GCC diagnostic ignored "-Wsign-compare"
#include "InfInt.h"
#pragma GCC diagnostic pop

#include "calc_factors.h"

using namespace std;

void usage(string msg="") {
    if (msg != "") {
        cerr << msg << endl;
    }

    cout << "Usage: factoring [OPTION]... [NUMBER]..." << endl;
    cout << "Options:" << endl;
    cout << "  --help          display this help and exit" << endl;
    cout << "  --async         run async in own thread" << endl;

    exit(1);
}

void output(vector<InfInt> nums, vector<shared_future<vector<InfInt>>> factors) {
    auto start = chrono::system_clock::now();
    for (size_t idx{}; idx < nums.size(); idx++) {
        vector<InfInt> f = factors[idx].get();
        cout << nums[idx] << ":  ";
        for (InfInt res : f) {
            cout << res << " ";
        }
        cout << endl;
    }
    auto end = chrono::system_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Time elapsed used for factoring: " << duration.count() << "ms" << endl;
}

void check(vector<InfInt> nums, vector<shared_future<vector<InfInt>>> factors) {
    for (size_t idx{}; idx < nums.size(); idx++) {
        vector<InfInt> f = factors[idx].get();
        InfInt p{1};
        for (InfInt res : f) {
            p *= res;
        }
        if (nums[idx] != p) {
            cerr << "factorisation failed for " << nums[idx] << "!" << p << endl;
        }
    }
}

int main(int argc, char* argv[]) {
    bool async_set{};
    if ((argc > 1) && strcmp(argv[1], "--help") == 0) {
        usage();
    } else if ((argc > 1) && strcmp(argv[1], "--async") == 0) {
        async_set = true;
    }

    vector<InfInt> v{};

    int i{1};
    if (async_set) {
        i = 2;
    }
    for (; i < argc; i++) {
        InfInt num = argv[i];
        if (num.toString() == argv[i]) {
            v.push_back(num);
        } else {
            string msg = argv[i];
            msg += " is not a number!";
            usage(msg);
        }
    }

    vector<shared_future<vector<InfInt>>> futures{};

    for (InfInt num : v) {
        if (async_set) {
            futures.push_back(async(launch::async, get_factors, num));
        } else {
            futures.push_back(async(get_factors, num));
        }

    }

    thread output_thread{output, v, futures};
    thread check_thread{check, v, futures};
    output_thread.join();
    check_thread.join();
}

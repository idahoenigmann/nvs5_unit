//
// Created by ida on 14.02.20.
//
#include <chrono>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <tuple>
#include <sstream>
#include <mutex>

#include "Clock.h"
#include "timeutils.h"

using namespace std;

mutex Clock::mtx;

void Clock::operator() () {
    while (true) {
        stringstream s;
        {
            unique_lock<mutex> l(mtx);
            chrono::seconds duration{1};
            curr_time += duration;
            s << name << ": " << curr_time << "\n";
        }
        cout << s.str() << flush;
        this_thread::sleep_for(chrono::milliseconds(int(clock_speed * slowdown_rate)));
    }
}

void Clock::set_time(int hours, int minutes, int seconds) {
    unique_lock<mutex> l(mtx);
    this->curr_time = ::set_time(this->curr_time, hours, minutes, seconds);
}

tuple<int, int, int> Clock::get_time() {
    unique_lock<mutex> l(mtx);
    return ::get_time(this->curr_time);
}

long Clock::to_time() {
    unique_lock<mutex> l(mtx);
    return std::chrono::system_clock::to_time_t(curr_time);
}

void Clock::from_time(long a) {
    unique_lock<mutex> l(mtx);
    curr_time = std::chrono::system_clock::from_time_t(a);
}

void Clock::set_clock_speed(int cs) {
    this->clock_speed = cs;
}

void Clock::set_time_monotone(long a) {
    auto new_time = std::chrono::system_clock::from_time_t(a);
    slowdown_rate = 1;
    if (new_time < this->curr_time) {
        {
            unique_lock<mutex> l(mtx);
            slowdown_rate = 0.5;
        }
    } else {
        unique_lock<mutex> l(mtx);
        curr_time = std::chrono::system_clock::from_time_t(a);
    }
}
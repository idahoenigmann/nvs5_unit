//
// Created by ida on 14.02.20.
//

#ifndef HOENIGMANN_I15013_CLOCK_H
#define HOENIGMANN_I15013_CLOCK_H

#include <mutex>

#include "timeutils.h"

class Clock {
public:
    // name of this clock
    Clock(std::string name) {
        this->name = name;
        this->curr_time = std::chrono::system_clock::now();
    }

    Clock(std::string name_, int hours_, int minutes_, int seconds_) {
        this->name = name_;
        this->curr_time = ::set_time(this->curr_time, hours_, minutes_, seconds_);
    }

    void set_time(int hours, int minutes, int seconds);
    std::tuple<int, int, int> get_time();

    long to_time();
    void from_time(long);

    void operator()();

    void set_clock_speed(int cs);

    void set_time_monotone(long a);

    std::chrono::time_point <std::chrono::system_clock> curr_time;
private:
    static std::mutex mtx;
    std::string name;
    int clock_speed{1000};
    double slowdown_rate{1};

};

#endif //HOENIGMANN_I15013_CLOCK_H

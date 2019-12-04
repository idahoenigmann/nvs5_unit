//
// Created by ida on 04.12.19.
//

#ifndef HOENIGMANN_I15013_PHILOSOPHER_H
#define HOENIGMANN_I15013_PHILOSOPHER_H

#include <mutex>

#include "Semaphore.h"

class Philosopher {
public:
    Philosopher(int id, std::mutex& left, std::mutex& right) : id(id), left(left), right(right) {}

    void operator()(Semaphore* semaphore, bool livelock);
private:
    int id;
    std::mutex& left;
    std::mutex& right;
};

#endif //HOENIGMANN_I15013_PHILOSOPHER_H

//
// Created by ida on 04.12.19.
//

#ifndef HOENIGMANN_I15013_SEMAPHORE_H
#define HOENIGMANN_I15013_SEMAPHORE_H

#include <mutex>
#include <condition_variable>

class Semaphore {
public:
    Semaphore() {
        cnt = 0;
    }

    Semaphore(int start_value) {
        cnt = start_value;
    }

    void acquire();
    void release();
    int available_permits();

private:
    int cnt{};
    std::mutex mtx;
    std::condition_variable not_zero;
};


#endif //HOENIGMANN_I15013_SEMAPHORE_H

//
// Created by ida on 04.12.19.
//

#include <mutex>

#include "Semaphore.h"

using namespace std;

void Semaphore::acquire() {
    unique_lock<mutex> lock{mtx};
    not_zero.wait(lock, [this]{ return cnt; });

    cnt--;
}

void Semaphore::release() {
    lock_guard<mutex> lock{mtx};

    cnt++;
    not_zero.notify_one();
}

int Semaphore::available_permits() {
    lock_guard<mutex> lock{mtx};

    return cnt;
}
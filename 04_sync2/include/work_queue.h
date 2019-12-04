//
// Created by ida on 04.12.19.
//

#ifndef HOENIGMANN_I15013_WORK_QUEUE_H
#define HOENIGMANN_I15013_WORK_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

#include "work_packet.h"

class WorkQueue {
public:
    WorkQueue(unsigned size) {
        this->size = size;
    }

    void push(WorkPacket workPacket);
    WorkPacket pop();

private:
    std::queue<WorkPacket> queue;
    std::mutex mtx;
    std::condition_variable not_empty;
    std::condition_variable not_full;
    unsigned size;
};

#endif //HOENIGMANN_I15013_WORK_QUEUE_H

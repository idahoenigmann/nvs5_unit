//
// Created by ida on 04.12.19.
//

#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>

#include "work_queue.h"
#include "work_packet.h"

using namespace std;

void WorkQueue::push(WorkPacket workPacket) {
    unique_lock<mutex> lock(mtx);
    not_full.wait(lock, [this]{return queue.size() < size;});
    queue.push(workPacket);
    not_empty.notify_one();
}

WorkPacket WorkQueue::pop() {
    unique_lock<mutex> lock(mtx);
    not_empty.wait(lock, [this]{return queue.size();});
    WorkPacket workPacket = queue.front();
    queue.pop();
    not_full.notify_one();
    return workPacket;
}
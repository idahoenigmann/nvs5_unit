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

#include "work_queue.h"
#include "work_packet.h"

using namespace std;

void worker(int id, WorkQueue& q) {
    ostringstream buf;
    random_device rd;
    mt19937 gen{rd()};
    uniform_real_distribution<> dis{0, 10};
    while (true) {
        buf << "W" << id << ": Want work packet" << endl;
        cout << buf.str();
        buf.str("");
        WorkPacket workPacket = q.pop();
        buf << "W" << id << ": Got work packet " << workPacket.get_id() << endl;
        cout << buf.str();
        buf.str("");
        auto ms = chrono::milliseconds{static_cast<int>(dis(gen) * 1000)};
        this_thread::sleep_for(ms);
        buf << "W" << id << ": Processed work packet " << workPacket.get_id() << " (" << float(ms.count()) / 1000 << "s)" << endl;
        cout << buf.str();
        buf.str("");
    }
}

int main(int argc, const char* argv[]) {
    WorkQueue workQueue{10};

    if (argc > 2) {
        workQueue.size = stoi(argv[1]);
    }

    thread w1{worker, 1, ref(workQueue)};
    thread w2{worker, 2, ref(workQueue)};
    thread w3{worker, 3, ref(workQueue)};

    int i{};
    ostringstream buf;
    random_device rd;
    mt19937 gen{rd()};
    uniform_real_distribution<> dis{0, 1};
    while (true) {
        WorkPacket workPacket{i};
        buf << "B: Waiting to submit work packet " << i << endl;
        cout << buf.str();
        buf.str("");

        auto ms = chrono::milliseconds{static_cast<int>(dis(gen) * 1000)};
        this_thread::sleep_for(ms);

        buf << "B: Submitted work packet " << i << " (" << float(ms.count()) / 1000 << "s)" << endl;
        cout << buf.str();
        buf.str("");

        workQueue.push(workPacket);

        i++;
    }

    w1.join();
    w2.join();
}

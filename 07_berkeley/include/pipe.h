#ifndef PIPE_H
#define PIPE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <future>
#include <thread>
#include <chrono>

template <typename T>
class Pipe {
    std::queue<T> backend;
    std::mutex mtx;
    std::condition_variable not_empty;
    bool closed{false};
    long latency{};
  public:
    Pipe& operator<<(T value) {
        std::this_thread::sleep_for(std::chrono::milliseconds(latency));
        std::unique_lock<std::mutex> l(mtx);
        backend.push(value);
        not_empty.notify_one();
        return *this;
    }
    
    Pipe& operator>>(T& value) {
        std::unique_lock<std::mutex> l(mtx);
        not_empty.wait(l, [this]{ return !backend.empty() or closed; });
        if (!closed) {
            value = backend.front();
            backend.pop();
        }
        return *this;
    }

    void close() {
        closed = true;
        not_empty.notify_all();
    }
    
    explicit operator bool() {
        return !closed;
    }

    void set_latency(long l) {
        this->latency = l;
    }
};
#endif

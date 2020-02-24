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

#include "CLI11.hpp"

#include "Clock.h"
#include "timeutils.h"
#include "pipe.h"

using namespace std;

class Channel {
public:
    Pipe<long>& get_pipe1() {
        return pipe1;
    }

    Pipe<long>& get_pipe2() {
        return pipe2;
    }

    void set_latency(long l) {
        pipe1.set_latency(l);
        pipe2.set_latency(l);
    }

private:
    Pipe<long> pipe1;
    Pipe<long> pipe2;
};

class TimeSlave {
  public:
    TimeSlave(string name, int hours, int minutes, int seconds, long latency=0, int clock_speed=1000, bool monotone=false) {
        this->clock = Clock(name, hours, minutes, seconds);
        this->channel = new Channel();
        this->channel->set_latency(latency);
        this->clock.set_clock_speed(clock_speed);
        this->monotone = monotone;
    }

    ~TimeSlave() {
        delete channel;
    }

    void operator() () {
        thread t_clock{ref(clock)};
        long value{};
        stringstream s;
        while (channel->get_pipe1() >> value) {
            s << "[client] received value: " << value << " from master\n";
            cout << s.str() << flush;
            s.str("");
            channel->get_pipe2() << clock.to_time();
            s << "[client] sent value: " << clock.to_time() << "\n";
            cout << s.str() << flush;
            s.str("");
            channel->get_pipe1() >> value;
            if (monotone) {
                clock.set_time_monotone(value);
            }
            else {
                clock.from_time(value);
            }
            s << "[client] set time to: " << clock.to_time() << "\n";
            cout << s.str() << flush;
            s.str("");
        }
        t_clock.join();
    }

    Channel* get_channel() {
        return channel;
    }

  private:
    Clock clock{""};
    Channel* channel{};
    bool monotone{};
};

class TimeMaster {
  public:
    TimeMaster(string name, int hours, int minutes, int seconds, int clock_speed=1000, bool monotone=false) {
        this->clock = Clock(name, hours, minutes, seconds);
        this->clock.set_clock_speed(clock_speed);
        this->monotone = monotone;
    }

    void operator() () {
        thread t_clock{ref(clock)};
        long time_slave1{};
        long time_slave2{};
        stringstream s;
        while (true) {
            this_thread::sleep_for(3s);
            channel1->get_pipe1() << clock.to_time();
            channel2->get_pipe1() << clock.to_time();
            s << "[master] received value: " << clock.to_time() << " from master\n";
            cout << s.str() << flush;
            s.str("");
            channel1->get_pipe2() >> time_slave1;
            s << "[master] received value: " << time_slave1 << " from slave1\n";
            cout << s.str() << flush;
            s.str("");
            channel2->get_pipe2() >> time_slave2;
            s << "[master] received value: " << time_slave2 << " from slave2\n";
            cout << s.str() << flush;
            s.str("");
            long avg = (time_slave1 + time_slave2 + clock.to_time()) / 3;
            s << "[master] calculated avg: " << avg << "\n";
            cout << s.str() << flush;
            s.str("");
            if (monotone) {
                clock.set_time_monotone(avg);
            }
            else {
                clock.from_time(avg);
            }
            s << "[master] set time to: " << clock.to_time() << "\n";
            cout << s.str() << flush;
            s.str("");
            channel1->get_pipe1() << avg;
            channel2->get_pipe1() << avg;
        }
        t_clock.join();
        channel1->get_pipe1().close();
        channel2->get_pipe1().close();
    }

    void set_channel1(Channel* c) {
        this->channel1 = c;
    }

    void set_channel2(Channel* c) {
        this->channel2 = c;
    }

  private:
    Clock clock{""};
    Channel* channel1{};
    Channel* channel2{};
    bool monotone{};
};

int main(int argc, char* argv[]) {
    CLI::App app("");

    bool monoton;
    vector<int> delay{0, 0};
    vector<int> rate{0, 0, 0};

    app.add_flag("-m", monoton, "monoton (kein Zurückstellen der Uhren)");
    app.add_option("-d", delay, "Verzögerungen in ms(d.h. Latenz der Verbindung zu jeweiligen Slave)")->expected(2);
    app.add_option("-r", rate, "Abweichungen vom Sekundentakt in ms (der jeweiligen Uhr, d.h. Ungenauigkeit)")->expected(3);

    CLI11_PARSE(app, argc, argv);

    TimeSlave slave1{"slave1", 15, 0, 0, delay[0], 1000+rate[0], monoton};
    TimeSlave slave2{"slave2", 14, 50, 0, delay[1], 1000+rate[1], monoton};
    TimeMaster master{"master", 15, 25, 0, 1000+rate[2], monoton};

    master.set_channel1(slave1.get_channel());
    master.set_channel2(slave2.get_channel());

    thread t1{slave1};
    thread t2{slave2};
    thread m{master};

    t1.join();
    t2.join();
    m.join();

    return 0;
}

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

using namespace std;

class Car {
public:
    Car(const string& model) {
        this->model = model;
    }

    void operator()() {
        int i{};
        random_device rd;
        mt19937 gen{rd()};
        uniform_real_distribution<> dis{1, 10};
        ostringstream buf;
        while (i < 10) {
            auto ms = chrono::milliseconds{static_cast<int>(dis(gen) * 1000)};
            this_thread::sleep_for(ms);
            total_time += static_cast<double>(chrono::milliseconds(ms).count()) / 1000;

            buf << ++i << " " << model << " " << setprecision(3) << static_cast<double>(chrono::milliseconds(ms).count()) / 1000 << "s\n";
            // cout << to_string(++i) + " " + model + " " + time + "s\n" << flush;
            cout << buf.str();
            buf.str("");
        }
    }

    double get_total_time() {
        return total_time;
    }

private:
    string model;
    double total_time{};
};

void car(double& total_time) {
    int i{};
    random_device rd;
    mt19937 gen{rd()};
    uniform_real_distribution<> dis{1, 10};
    ostringstream buf;
    total_time = 0;
    while (i < 10) {
        auto ms = chrono::milliseconds{static_cast<int>(dis(gen) * 1000)};
        this_thread::sleep_for(ms);
        total_time += static_cast<double>(chrono::milliseconds(ms).count()) / 1000;

        buf << ++i << " car " << setprecision(3) << static_cast<double>(chrono::milliseconds(ms).count()) / 1000 << "s\n";
        // cout << to_string(++i) + " " + model + " " + time + "s\n" << flush;
        cout << buf.str();
        buf.str("");
    }
}

int main() {
    Car car2{"car2"};
    double total_time_car{};
    thread t{car, ref(total_time_car)};
    thread t2{ref(car2)};
    t.join();
    t2.join();

    double total_time_car2{car2.get_total_time()};

    if (total_time_car < total_time_car2) {
        cout << "Sieger ist: car mit " << total_time_car << "s" << endl;
        cout << "Verlierer ist: car2 mit " << total_time_car2 << "s" << endl;
    } else {
        cout << "Sieger ist: car2 mit " << total_time_car2 << "s" << endl;
        cout << "Verlierer ist: car mit " << total_time_car << "s" << endl;
    }


    return 0;
}

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

#include <asio.hpp>
#include <spdlog/spdlog.h>
#include <CLI11.hpp>

using namespace std;
using namespace asio::ip;

int main(int argc, char* argv[]) {
    CLI::App app{"prints the current date and time"};

    string port = "1113";
    app.add_option("-p,--port", port, "port to connect to");

    CLI11_PARSE(app, argc, argv);

    tcp::iostream strm{"localhost", port};
    if (strm) {
        spdlog::info("connection created");
        string data;
        getline(strm, data);
        spdlog::info("received data");
        cout << data << endl;
        strm.close();
        spdlog::info("connection closed");
    } else {
        spdlog::error("Could not connect to server!");
    }
    return 0;
}

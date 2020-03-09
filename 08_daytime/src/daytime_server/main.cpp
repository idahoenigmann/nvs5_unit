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
#include <ctime>

#include <asio.hpp>
#include <spdlog/spdlog.h>
#include <CLI11.hpp>

using namespace std;
using namespace asio::ip;

int main(int argc, char* argv[]) {
    CLI::App app{"sends the current date and time"};

    short unsigned int port;
    app.add_option("-p,--port", port, "server port")->required();

    CLI11_PARSE(app, argc, argv);

    while (true) {
        asio::io_context ctx;
        tcp::endpoint ep{tcp::v4(), port};
        spdlog::info("connection opened");
        tcp::acceptor acceptor{ctx, ep};
        spdlog::info("listening");
        acceptor.listen();

        tcp::socket sock{ctx};
        acceptor.accept(sock);
        tcp::iostream strm{std::move(sock)};

        if (strm) {
            auto date{chrono::system_clock::to_time_t(asio::chrono::system_clock::now())};
            spdlog::info("sending current date and time");
            strm << ctime(&date) << flush;
            strm.close();
        } else {
            spdlog::error("could not open stream");
        }
        spdlog::info("connection closed");
    }
    return 0;
}

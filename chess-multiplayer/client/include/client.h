#pragma once
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <core/include/core.h>

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE
#include <asio.hpp>

using namespace asio::ip;

namespace client
{
    void mainloop();

    void receive(tcp::socket& sock, std::mutex& mtx, bool& running);

    void send(tcp::socket& sock, const std::string& msg);

    void setup_board();
    bool connect_to_server(tcp::socket& sock, Color& color);

    void handle_new_move(const std::string& data);

    std::string sock_read(tcp::socket& sock);
}
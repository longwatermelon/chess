#pragma once
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <memory>

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE
#include <asio.hpp>

using namespace asio::ip;

namespace server
{
    inline std::vector<std::unique_ptr<tcp::socket>> m_users;

    void receive(std::mutex& mtx);

    void send(tcp::socket& sock, const std::string& msg);

    void broadcast(const std::string& msg, tcp::socket* ignored = nullptr);

    void accept_users(std::mutex& mtx, tcp::acceptor& act, asio::io_service& service, std::string& next_color);
}
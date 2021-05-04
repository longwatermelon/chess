#include "../include/server.h"
#include <mutex>


void server::receive(std::mutex& mtx)
{
    while (true)
    {
        {
            std::lock_guard lock(mtx);

            std::string data;

            for (int i = 0; i < m_users.size(); ++i)
            {
                size_t bytes = m_users[i]->available();

                if (bytes > 0)
                {
                    std::cout << "read " << bytes << " bytes\n";

                    std::vector<char> buf(bytes);
                    m_users[i]->read_some(asio::buffer(buf.data(), buf.size()));

                    std::string data;
                    for (char c : buf)
                        data += c;

                    std::cout << "received: " << data << "\n";
                    broadcast(data, m_users[i].get());
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}


void server::send(tcp::socket& sock, const std::string& msg)
{
    sock.write_some(asio::buffer(msg.data(), msg.size()));
}


void server::broadcast(const std::string& msg, tcp::socket* ignored)
{
    for (auto& sock : m_users)
    {
        if (sock.get() == ignored)
            continue;

        send(*sock, msg);
    }
}


void server::accept_users(std::mutex& mtx, tcp::acceptor& act, asio::io_service& service, std::string& next_color)
{
    bool full_game = false;

    while (true)
    {
        if (m_users.size() >= 2 && !full_game)
        {
            std::cout << "found 2 users, starting game\n";

            for (auto& sock : m_users)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                send(*sock, "type=game-start");

                std::cout << "started socket " << sock.get() << "\n";
            }
        }

        std::unique_ptr<tcp::socket> sock = std::make_unique<tcp::socket>(service);

        std::cout << "listening for new users\n";
        act.accept(*sock);
        
        {
            std::lock_guard lock(mtx);

            if (full_game)
            {
                send(*sock, "type=error&message=already two players in game");
                continue;
            }

            send(*sock, "type=new-connect&color=" + next_color);
            m_users.emplace_back(std::move(sock));
            
            std::cout << "current user count: " << m_users.size() << "\n";

            if (next_color == "white")
                next_color = "black";
        }
    }
}
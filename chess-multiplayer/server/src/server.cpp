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
                size_t bytes = m_users[i]->sock()->available();

                if (bytes > 0)
                {
                    std::cout << "read " << bytes << " bytes\n";

                    std::vector<char> buf(bytes);
                    m_users[i]->sock()->read_some(asio::buffer(buf.data(), buf.size()));

                    std::string data;
                    for (char c : buf)
                        data += c;

                    std::cout << "received: " << data << "\n";

                    if (data == "ready")
                        m_users[i]->set_ready(true);
                    else
                        broadcast(data, m_users[i]->sock());
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
    for (auto& user : m_users)
    {
        if (user->sock() == ignored)
            continue;

        send(*(user->sock()), msg);
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

            for (auto& user : m_users)
            {
                while (!user->ready())
                    ;

                send(*(user->sock()), "type=game-start");

                std::cout << "started socket " << user->sock() << "\n";
            }
        }

        std::unique_ptr<tcp::socket> sock = std::make_unique<tcp::socket>(service);
        std::unique_ptr<User> user = std::make_unique<User>(std::move(sock), false);

        std::cout << "listening for new users\n";
        act.accept(*(user->sock()));
        
        {
            std::lock_guard lock(mtx);

            if (full_game)
            {
                send(*(user->sock()), "type=error&message=already two players in game");
                continue;
            }

            send(*(user->sock()), "type=new-connect&color=" + next_color);
            m_users.emplace_back(std::move(user));
            
            std::cout << "current user count: " << m_users.size() << "\n";

            if (next_color == "white")
                next_color = "black";
        }
    }
}
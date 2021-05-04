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


void server::accept_users(std::mutex& mtx, tcp::acceptor& act, asio::io_service& service)
{
	while (true)
	{
		if (m_users.size() >= 2)
		{
			std::cout << "found 2 users\n";
			break;
		}

		std::unique_ptr<tcp::socket> sock = std::make_unique<tcp::socket>(service);

		std::cout << "listening for new users\n";
		act.accept(*sock);

		{
			std::lock_guard lock(mtx);
			m_users.emplace_back(std::move(sock));
		}
	}
}
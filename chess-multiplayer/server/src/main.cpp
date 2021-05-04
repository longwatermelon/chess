#include "../include/server.h"
#include <thread>


int main(int argc, char** argv)
{
	asio::io_service service;
	tcp::acceptor act(service, tcp::endpoint(tcp::v4(), 1234));

	std::mutex mtx;

	std::thread thr_recv(server::receive, std::ref(mtx));
	std::thread thr_act(server::accept_users, std::ref(mtx), std::ref(act), std::ref(service));

	std::cout << "server is running\n";

	while (true);

	return 0;
}
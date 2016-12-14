#include <signal.h>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

std::string make_daytime_string()
{
	using namespace std;
	time_t now = time(0);
	return ctime(&now);
}

std::vector<tcp::endpoint> client_endpoints;

void quit_handler(sig_t s)
{
	std::cout << "Endpoints:" << std::endl;
	for (auto ep : client_endpoints)
		std::cout << ep << std::endl;
	exit(0);
}

int main()
{
	signal(SIGINT,  (void (*)(int)) quit_handler);
	
	try {
		boost::asio::io_service io_service;
		tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 13));
		std::cout << "Server started at port 13" << std::endl;
		while (true) {
			tcp::socket socket(io_service);
			acceptor.accept(socket);
			// request accepted.
			// get client IP
			tcp::endpoint remote_ep = socket.remote_endpoint();
			client_endpoints.push_back(remote_ep);
			boost::asio::ip::address remote_ad = remote_ep.address();
			std::string s = remote_ad.to_string();
			std::cout << "Created socket with " << s << std::endl;
			// return date & time
			std::string msg = make_daytime_string();
			boost::system::error_code ignored_error;
			boost::asio::write(socket, boost::asio::buffer(msg), ignored_error);
		}
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}

#include <iostream>
#include <boost/asio.hpp>

void signal_handler(const boost::system::error_code& e, int signal)
{
    std::cout << "Application terminated" << std::endl;
}

int main()
{
    boost::asio::io_service service;
    boost::asio::signal_set sig(service, SIGINT, SIGTERM);
    sig.async_wait(signal_handler);
    
    std::cout << "Application started" << std::endl;
    std::cout << "Press Ctrl+C for termination" << std::endl << std::endl;

    service.run();
    return 0;
}


// main function of Client

#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>

#include "client_headers/Client.hpp"
#include "client_headers/ClientApp.hpp"
#include "lib_headers/Message.hpp"

using boost::asio::ip::tcp;

using namespace meow::client;
using namespace meow;

int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: chat_client <host> <port>\n";
            return 1;
        }
        
        ClientApp app;
        app.main(argc-1, ++argv);

    }
    catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << endl;
    }

    return 0;
}

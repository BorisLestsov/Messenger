// main function of Client

#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>

#include "Client.hpp"
#include "Message.hpp"

using boost::asio::ip::tcp;

using namespace meow;

int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: chat_client <host> <port>\n";
            return 1;
        }


        boost::asio::io_service io_service;

        tcp::resolver resolver(io_service);
        auto endpoint_iterator = resolver.resolve({argv[1], argv[2]});

        Client c(io_service, endpoint_iterator);

        std::thread t([&io_service]() {
            io_service.run();
        });

        char line[Message::MAX_BODY_LENGTH + 1];

        cout << "Please, type messages:" << endl;
        while (std::cin.getline(line, Message::MAX_BODY_LENGTH + 1)) {
            Message msg;
            msg.body_length(std::strlen(line));
            std::memcpy(msg.body(), line, msg.body_length());
            msg.encode_header();
            c.write(msg);
        }

        c.close();
        t.join();
    }
    catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
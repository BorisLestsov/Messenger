//
// chat_server.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <boost/asio.hpp>

#include "chat_participant.hpp"
#include "chat_session.hpp"
#include "chat_room.hpp"
#include "chat_message.hpp"
#include "Server.hpp"


using boost::asio::ip::tcp;
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using namespace meow;


int main(int argc, char* argv[])
{
    try {
        if (argc < 2) {
            std::cerr << "Usage: ./Server <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;
            
        // only one server thread is allowed!
        tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[1]));
        Server server(io_service, endpoint);
        cout << "Created server at endpoint: " << endpoint << endl;
        
        io_service.run();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}

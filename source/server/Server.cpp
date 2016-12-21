#include <boost/asio.hpp>
#include <iostream>
#include "server_headers/Server.hpp"

namespace meow {
    namespace server {

        using boost::asio::ip::tcp;

        Server::Server(boost::asio::io_service& io_service, const tcp::endpoint &endpoint) :
                acceptor_(io_service, endpoint),
                server_socket_(io_service),
                rooms_()
        {
            rooms_.push_back(Chatroom(io_service));
            do_accept();
        }

        size_t Server::n_rooms() const {
            return rooms_.size();
        }

        void Server::do_accept() {
            auto acceptor_f = [this](boost::system::error_code error_code) {
                if (!error_code)
                    std::make_shared<Session>(rooms_.back())->start();
                else
                    cerr << "Error Code in do_accept" << endl;
                do_accept();
            };

            // Connect to last and only chatroom for now
            acceptor_.async_accept(*(rooms_.back().get_chatroom_socket()), acceptor_f);
        }

    }
} // namespace meow


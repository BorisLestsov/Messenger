#ifndef _SERVER_HPP_INCLUDED
#define _SERVER_HPP_INCLUDED

#include <boost/asio.hpp>

#include "Participant.hpp"
#include "Session.hpp"
#include "Chatroom.hpp"
#include "lib_headers/Message.hpp"

namespace meow {
    namespace server {

        using boost::asio::ip::tcp;

        class Chatroom;

        class Server {
        public:
            Server(boost::asio::io_service &io_service, const tcp::endpoint &endpoint);

            size_t n_opened() const;  // number of clients
        private:
            void do_accept();

            tcp::acceptor acceptor_;
            tcp::socket socket_;
            Chatroom room_;
        };

    }
}

#endif // _SERVER_HPP_INCLUDED

#pragma once
#ifndef _SERVER_HPP_INCLUDED
#define _SERVER_HPP_INCLUDED

#include <boost/asio.hpp>

#include "chat_participant.hpp"
#include "chat_session.hpp"
#include "chat_room.hpp"
#include "chat_message.hpp"

namespace meow {

using boost::asio::ip::tcp;

class chat_room;

class Server {
public:
    Server(boost::asio::io_service& io_service, const tcp::endpoint& endpoint);
private:
    void do_accept();

    tcp::acceptor acceptor_;
    tcp::socket socket_;
    chat_room room_;
};

}

#endif // _SERVER_HPP_INCLUDED

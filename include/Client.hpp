#ifndef _CLIENT_HPP_INCLUDED
#define _CLIENT_HPP_INCLUDED

#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>

#include "Message.hpp"

namespace meow {

    using boost::asio::ip::tcp;

    typedef std::deque<SerializedMessage> chat_message_queue;

    class Client {
    public:
        Client(boost::asio::io_service &io_service,
               tcp::resolver::iterator endpoint_iterator);

        void write(const Message &msg);

        void close();

    private:
        void do_connect(tcp::resolver::iterator endpoint_iterator);

        void do_read_header();

        void do_read_body();

        void do_write();

    private:
        boost::asio::io_service &io_service_;
        tcp::socket socket_;
        SerializedMessage msg_buf_;
        chat_message_queue write_msgs_;
    };

} // namespace meow

#endif // _CLIENT_HPP_INCLUDED

#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>

#include "Message.hpp"
#include "Client.hpp"

namespace meow {

    /*  Вообщем, я вроде бы исправил write, do_write, но пока не тестил
     *
     *  Что нужно сделать:
     *  Придумать как будет работать read и все с ним связанное
     *  По идее, нужно читать из сокета в какой-то буфер, а потом восстановить из него
     *  исходное сообщение.
     *  Конструктор, это делающий, уже есть в Message, но он принимает
     *  SerializedMessage.
     *
     */


    using boost::asio::ip::tcp;

    typedef std::deque<Message> chat_message_queue;

    Client::Client(boost::asio::io_service &io_service,
                   tcp::resolver::iterator endpoint_iterator):
            io_service_(io_service),
            socket_(io_service),
            msg_buf()
    {
        msg_buf.resize(SerializedMessage::MAX_MSG_LENGTH);
        do_connect(endpoint_iterator);
    }

    void Client::write(const Message &msg) {
        auto write_f = [this, msg]() {
            bool write_in_progress = !write_msgs_.empty();

            write_msgs_.push_back(msg);
            if (!write_in_progress)
                do_write();
        };

        io_service_.post(write_f);
    }

    void Client::close() {
        io_service_.post([this]() { socket_.close(); });
    }

    void Client::do_connect(tcp::resolver::iterator endpoint_iterator) {
        auto connect_f = [this](boost::system::error_code ec, tcp::resolver::iterator) {
            if (!ec)
                do_read_header();
        };

        boost::asio::async_connect(socket_,
                                   endpoint_iterator,
                                   connect_f
        );
    }

    void Client::do_read_header() {
        auto read_header_f = [this](boost::system::error_code ec,
                                             std::size_t length) {
            if (!ec) {
                msg_buf.decode_msg_length();
                do_read_body();
            } else
                socket_.close();
        };

        boost::asio::async_read(socket_,
                                boost::asio::buffer(msg_buf.get_buf(),
                                SerializedMessage::HEADER_LENGTH),
                                read_header_f
        );
    }

    void Client::do_read_body() {
        auto read_body_f = [this](boost::system::error_code ec,
                                           std::size_t length) {
            if (!ec) {
                std::cout.write(msg_buf.get_body_buf(), msg_buf.get_body_len());
                std::cout << endl;
                do_read_header();
            } else
                socket_.close();
        };


        boost::asio::async_read(socket_,
                                boost::asio::buffer(read_msg_.body(),
                                read_msg_.body_length()),
                                read_body_f
        );
    }

    void Client::do_write() {
        auto write_f = [this](boost::system::error_code ec,
                                       std::size_t length) {
            if (!ec) {
                write_msgs_.pop_front();
                if (!write_msgs_.empty()) {
                    do_write();
                }
            } else
                socket_.close();
        };

        SerializedMessage msg_buf = write_msgs_.front().serialize();

        boost::asio::async_write(socket_,
                                 boost::asio::buffer(msg_buf.get_buf(),
                                 msg_buf.get_msg_len()),
                                 write_f
        );
    }


} // namespace meow


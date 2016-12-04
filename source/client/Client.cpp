#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>

#include "chat_message.hpp"
#include "Client.hpp"

namespace meow {

using boost::asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

Client::Client(boost::asio::io_service& io_service,
	tcp::resolver::iterator endpoint_iterator)
    : io_service_(io_service), socket_(io_service)
{
    do_connect(endpoint_iterator);
}

void Client::write(const chat_message& msg)
{
    io_service_.post([this, msg]() {
		bool write_in_progress = !write_msgs_.empty();
        write_msgs_.push_back(msg);
        if (!write_in_progress)
            do_write();
    });
}

void Client::close()
{
    io_service_.post([this]() { socket_.close(); });
}

void Client::do_connect(tcp::resolver::iterator endpoint_iterator)
{
    boost::asio::async_connect(socket_, endpoint_iterator,
        [this](boost::system::error_code ec, tcp::resolver::iterator) {
			if (!ec)
				do_read_header();
    });
}

void Client::do_read_header()
{
    boost::asio::async_read(socket_,
        boost::asio::buffer(read_msg_.data(), chat_message::header_length),
        [this](boost::system::error_code ec, std::size_t /*length*/) {
			if (!ec && read_msg_.decode_header()) {
				do_read_body();
			} else {
				socket_.close();
			}
    });
}

void Client::do_read_body()
{
    boost::asio::async_read(socket_,
        boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
        [this](boost::system::error_code ec, std::size_t /*length*/) {
			if (!ec) {
				std::cout.write(read_msg_.body(), read_msg_.body_length());
				std::cout << "\n";
				do_read_header();
			} else {
				socket_.close();
			}
    });
}

void Client::do_write()
{
    boost::asio::async_write(socket_,
	boost::asio::buffer(write_msgs_.front().data(),
		write_msgs_.front().length()),
        [this](boost::system::error_code ec, std::size_t /*length*/) {
        if (!ec) {
            write_msgs_.pop_front();
            if (!write_msgs_.empty()) {
              do_write();
            }
        } else {
            socket_.close();
        }
    });
}


} // namespace meow


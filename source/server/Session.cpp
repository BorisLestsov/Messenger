#include <boost/asio.hpp>

#include "Chatroom.hpp"
#include "Message.hpp"
#include "Session.hpp"

namespace meow {

using boost::asio::ip::tcp;

Session::Session(tcp::socket socket, Chatroom& room)
    : socket_(std::move(socket)),
      room_(room)
{
}

void Session::start()
{
	room_.join(shared_from_this());
	do_read_header();
}

void Session::deliver(const Message& msg)
{
	bool write_in_progress = !write_msgs_.empty();
	write_msgs_.push_back(msg);
	if (!write_in_progress)
		do_write();
}


void Session::do_read_header()
{
	auto self(shared_from_this());
	boost::asio::async_read(socket_,
		boost::asio::buffer(read_msg_.data(), Message::HEADER_LENGTH),
			[this, self](boost::system::error_code ec, std::size_t /*length*/) {
				if (!ec && read_msg_.decode_header())
					do_read_body();
				else
					room_.leave(shared_from_this());
        });
}

void Session::do_read_body()
{
    auto self(shared_from_this());
    boost::asio::async_read(socket_,
        boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
			[this, self](boost::system::error_code ec, std::size_t /*length*/) {
				if (!ec) {
					room_.deliver(read_msg_);
					do_read_header();
				} else {
					room_.leave(shared_from_this());
				}
        });
}

void Session::do_write()
{
    auto self(shared_from_this());
    boost::asio::async_write(socket_,
        boost::asio::buffer(write_msgs_.front().data(),
			write_msgs_.front().length()),
			[this, self](boost::system::error_code ec, std::size_t /*length*/) {
				if (!ec) {
					write_msgs_.pop_front();
					if (!write_msgs_.empty())
						do_write();
				} else {
					room_.leave(shared_from_this());
				}
        });
}

} // namespace meow


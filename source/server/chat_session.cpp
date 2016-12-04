#include <boost/asio.hpp>

#include "chat_room.hpp"
#include "chat_message.hpp"
#include "chat_session.hpp"

namespace meow {

using boost::asio::ip::tcp;

chat_session::chat_session(tcp::socket socket, chat_room& room)
    : socket_(std::move(socket)),
      room_(room)
{
}

void chat_session::start()
{
	room_.join(shared_from_this());
	do_read_header();
}

void chat_session::deliver(const chat_message& msg)
{
	bool write_in_progress = !write_msgs_.empty();
	write_msgs_.push_back(msg);
	if (!write_in_progress)
		do_write();
}


void chat_session::do_read_header()
{
	auto self(shared_from_this());
	boost::asio::async_read(socket_,
		boost::asio::buffer(read_msg_.data(), chat_message::header_length),
			[this, self](boost::system::error_code ec, std::size_t /*length*/) {
				if (!ec && read_msg_.decode_header())
					do_read_body();
				else
					room_.leave(shared_from_this());
        });
}

void chat_session::do_read_body()
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

void chat_session::do_write()
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


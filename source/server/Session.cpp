#include <boost/asio.hpp>
#include <iostream>

#include "Chatroom.hpp"
#include "Message.hpp"
#include "Session.hpp"

namespace meow {

	using boost::asio::ip::tcp;

	Session::Session(tcp::socket socket, Chatroom &room)
			: socket_(std::move(socket)),
			  room_(room),
              msg_buf_()
    {
        msg_buf_.resize(SerializedMessage::MAX_MSG_LENGTH);
	}

	void Session::start() {
		room_.join(shared_from_this());
		do_read_header();
	}

	void Session::deliver(const SerializedMessage &msg) {
		bool write_in_progress = !write_msgs_.empty();
		write_msgs_.push_back(msg);
		if (!write_in_progress)
			do_write();
	}


	void Session::do_read_header() {
		auto self(shared_from_this());

		auto read_header_f = [this, self](boost::system::error_code ec, std::size_t /*length*/) {
			if (!ec) {
                msg_buf_.decode_msg_length();
                do_read_body();
            } else {
                std::cout << "ec in session::do_read_header" << endl;
                room_.leave(shared_from_this());
            }
		};

		boost::asio::async_read(socket_,
                                boost::asio::buffer(msg_buf_.get_buf(),
                                SerializedMessage::HEADER_LENGTH),
                                read_header_f
        );
	}

	void Session::do_read_body() {
		auto self(shared_from_this());
        auto read_body_f = [this, self](boost::system::error_code ec, std::size_t /*length*/) {
            if (!ec) {
                Message msg(msg_buf_);
                room_.deliver(msg);
                do_read_header();
            } else {
                std::cout << "ec in session::do_read_body" << endl;
                room_.leave(shared_from_this());
            }
        };

		boost::asio::async_read(socket_,
								boost::asio::buffer(msg_buf_.get_body_buf(),
                                msg_buf_.get_body_len()),
								read_body_f
        );
	}

	void Session::do_write() {
		auto self(shared_from_this());

        auto write_f = [this, self](boost::system::error_code ec, std::size_t /*length*/) {
            if (!ec) {
                write_msgs_.pop_front();
                if (!write_msgs_.empty())
                    do_write();
            } else {
				std::cout << "ec in session::do_write" << endl;
                room_.leave(shared_from_this());
            }
        };

        SerializedMessage msg_buf = write_msgs_.front();

        boost::asio::async_write(socket_,
								 boost::asio::buffer(msg_buf.get_buf(),
													 msg_buf.get_msg_len()),
                                 write_f

        );
	}

} // namespace meow


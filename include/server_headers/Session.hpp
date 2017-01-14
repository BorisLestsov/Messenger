#ifndef _CHAT_SESSION_HPP_INCLUDED
#define _CHAT_SESSION_HPP_INCLUDED

#include <boost/asio.hpp>

#include "Participant.hpp"
#include "Chatroom.hpp"
#include "server_headers/Server.hpp"
#include "lib_headers/Message.hpp"

namespace meow {
	namespace server {

		using boost::asio::ip::tcp;

		class Session
				: public Participant,
				  public std::enable_shared_from_this<Session> {
		public:
			Session(Server* server, Chatroom &room);

			void start();

			void deliver(const SerializedMessage &msg) override;

		private:
			Server* server_;

			void do_read_header();

			void do_read_body();

			void do_write();

			tcp::socket socket_;
			Chatroom &room_;
			SerializedMessage msg_buf_;
			chat_message_queue write_msgs_;
		};

	}
} // namespace meow

#endif // _CHAT_SESSION_HPP_INCLUDED

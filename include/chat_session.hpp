#pragma once
#ifndef _CHAT_SESSION_HPP_INCLUDED
#define _CHAT_SESSION_HPP_INCLUDED

#include <boost/asio.hpp>

#include "chat_participant.hpp"
#include "chat_room.hpp"
#include "chat_message.hpp"

namespace meow {

using boost::asio::ip::tcp;

class chat_session
  : public ChatParticipant,
    public std::enable_shared_from_this<chat_session>
{
public:
	chat_session(tcp::socket socket, chat_room& room);

	void start();
	void deliver(const chat_message& msg);

private:
	void do_read_header();
	void do_read_body();
	void do_write();

	tcp::socket socket_;
	chat_room& room_;
	chat_message read_msg_;
	chat_message_queue write_msgs_;
};

} // namespace meow

#endif // _CHAT_SESSION_HPP_INCLUDED

#pragma once
#ifndef _CHAT_SESSION_HPP_INCLUDED
#define _CHAT_SESSION_HPP_INCLUDED

#include <boost/asio.hpp>

#include "Participant.hpp"
#include "Chatroom.hpp"
#include "Message.hpp"

namespace meow {

using boost::asio::ip::tcp;

class Session
  : public ChatParticipant,
    public std::enable_shared_from_this<Session>
{
public:
	Session(tcp::socket socket, Chatroom& room);

	void start();
	void deliver(const Message& msg);

private:
	void do_read_header();
	void do_read_body();
	void do_write();

	tcp::socket socket_;
	Chatroom& room_;
	Message read_msg_;
	chat_message_queue write_msgs_;
};

} // namespace meow

#endif // _CHAT_SESSION_HPP_INCLUDED

#pragma once
#ifndef _CHAT_ROOM_HPP_INCLUDED
#define _CHAT_ROOM_HPP_INCLUDED

#include <set>

#include "chat_participant.hpp"
#include "chat_message.hpp"

namespace meow {

class chat_room
{
public:
	void join(chat_participant_ptr participant);
	void leave(chat_participant_ptr participant);
	void deliver(const chat_message& msg);
	size_t size() const;
private:
	std::set<chat_participant_ptr> participants_;
	enum { max_recent_msgs = 100 };
	chat_message_queue recent_msgs_;
};

} // namespace meow

#endif // _CHAT_ROOM_HPP_INCLUDED

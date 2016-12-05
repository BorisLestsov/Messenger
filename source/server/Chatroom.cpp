#include <set>

#include "Participant.hpp"
#include "Message.hpp"
#include "Chatroom.hpp"

namespace meow {

void Chatroom::join(chat_participant_ptr participant)
{
    participants_.insert(participant);
    for (auto msg: recent_msgs_)
      participant->deliver(msg);   // send all recent messages to a new participant of chat
}

void Chatroom::leave(chat_participant_ptr participant)
  {
    participants_.erase(participant);
}

void Chatroom::deliver(const Message& msg)
{
    recent_msgs_.push_back(msg);
    while (recent_msgs_.size() > max_recent_msgs)
		recent_msgs_.pop_front();

	for (auto participant: participants_)  // send to all clients in this room
		participant->deliver(msg);
}

size_t Chatroom::size() const
{
	return participants_.size();
}

} // namespace meow


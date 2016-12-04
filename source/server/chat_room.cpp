#include <set>

#include "chat_participant.hpp"
#include "chat_message.hpp"
#include "chat_room.hpp"

namespace meow {

void chat_room::join(chat_participant_ptr participant)
{
    participants_.insert(participant);
    for (auto msg: recent_msgs_)
      participant->deliver(msg);   // send all recent messages to a new participant of chat
}

void chat_room::leave(chat_participant_ptr participant)
  {
    participants_.erase(participant);
}

void chat_room::deliver(const chat_message& msg)
{
    recent_msgs_.push_back(msg);
    while (recent_msgs_.size() > max_recent_msgs)
		recent_msgs_.pop_front();

	for (auto participant: participants_)  // send to all clients in this room
		participant->deliver(msg);
}

} // namespace meow


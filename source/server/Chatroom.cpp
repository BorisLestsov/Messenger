#include <set>

#include "server_headers/Participant.hpp"
#include "lib_headers/Message.hpp"
#include "server_headers/Chatroom.hpp"

namespace meow {
    namespace server {

        const uint64_t Chatroom::max_recent_msgs = MAX_RECENT_MESSAGES;

        Chatroom::Chatroom(boost::asio::io_service &io_service):
                chatroom_socket_(io_service)
        {}


        void Chatroom::join(chat_participant_ptr participant) {
            participants_.insert(participant);
            for (auto msg: recent_msgs_)
                participant->deliver(msg);   // send all recent messages to a new participant of chat
        }

        void Chatroom::leave(chat_participant_ptr participant) {
            participants_.erase(participant);
        }

        void Chatroom::deliver(const SerializedMessage &msg) {
            recent_msgs_.push_back(msg);
            while (recent_msgs_.size() > max_recent_msgs)
                recent_msgs_.pop_front();

            for (auto participant: participants_)  // send to all clients in this room
                participant->deliver(msg);
        }

        size_t Chatroom::size() const {
            return participants_.size();
        }

        tcp::socket* Chatroom::get_chatroom_socket() {
            return &chatroom_socket_;
        }

    }
} // namespace meow


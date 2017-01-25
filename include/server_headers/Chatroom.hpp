#ifndef _CHAT_ROOM_HPP_INCLUDED
#define _CHAT_ROOM_HPP_INCLUDED

#include <set>
#include <boost/asio.hpp>

#include "configs.h"
#include "Participant.hpp"
#include "ChatroomData.hpp"
#include "lib_headers/Message.hpp"

namespace meow {
    namespace server {

        using boost::asio::ip::tcp;

        class Chatroom {
        public:
            typedef ChatroomData::roomid_t room_id;

            Chatroom(boost::asio::io_service &io_service);

            void join(chat_participant_ptr participant);

            void leave(chat_participant_ptr participant);

            void deliver(const SerializedMessage &msg);

            tcp::socket* get_chatroom_socket();     // Пока очень костыльный метод

            size_t size() const;

        private:
//            const tcp::socket to_server_socket_;
            tcp::socket chatroom_socket_;
            static const uint max_recent_msgs;

            std::set <chat_participant_ptr> participants_;
            chat_message_queue recent_msgs_;
        };

    }
} // namespace meow

#endif // _CHAT_ROOM_HPP_INCLUDED

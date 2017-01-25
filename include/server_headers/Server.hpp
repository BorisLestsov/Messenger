#ifndef _SERVER_HPP_INCLUDED
#define _SERVER_HPP_INCLUDED

#include <boost/asio.hpp>
#include <map>
#include <vector>

#include "Participant.hpp"
#include "Chatroom.hpp"
#include "server_headers/ServerData.hpp"
#include "lib_headers/Message.hpp"

namespace meow {
    namespace server {

        using boost::asio::ip::tcp;
        using std::vector;

        class Chatroom;

        class Server {
        public:
            Server(boost::asio::io_service& io_service, const tcp::endpoint &endpoint);

            size_t n_rooms() const;  // number of clients
            void list_rooms() const;  // number of clients
            size_t n_in_room() const;
            void list_room() const;

            ServerDatabase* get_db();
            void deliver(const SerializedMessage& msg, Message::uid_t to);

        private:
            void do_accept();

            tcp::acceptor acceptor_;
            tcp::socket server_socket_;
            vector<Chatroom> rooms_;

            // user database
            ServerDatabase* db_;

            // for many chatrooms
            //std::map<Chatroom::room_id, Chatroom> rooms_map_;
            std::map<Participant::uid, chat_participant_ptr> users_map_;
        };
    }
}

#endif // _SERVER_HPP_INCLUDED

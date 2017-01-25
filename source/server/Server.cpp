#include <boost/asio.hpp>
#include <iostream>
#include "server_headers/Server.hpp"
#include "server_headers/ServerData.hpp"
#include "server_headers/Session.hpp"

namespace meow {
    namespace server {

        using boost::asio::ip::tcp;

        Server::Server(boost::asio::io_service& io_service, const tcp::endpoint &endpoint) :
                acceptor_(io_service, endpoint),
                server_socket_(io_service),
                rooms_()
        {
            db_ = new ServerDatabase();
            db_->load("meow-users.db");
            rooms_.push_back(Chatroom(io_service));
            do_accept();
        }

        size_t Server::n_rooms() const {
            return rooms_.size();
        }

        ServerDatabase* Server::get_db()
        {
            return db_;
        }

        void Server::deliver(const SerializedMessage& msg, Message::uid_t to)
        {
            ChatroomData* data = db_->get_room(to);

            /*
            // if global
            // terrible code
            if (data == db_->get_global_room()) {
                auto& full_map = db_->get_accounts_map();
                for (auto p = full_map.begin(); p != full_map.end(); p++) {
                    AccountData *acc = p->second;
                    Session *s = acc->get_session();
                    if (s) {
                        s->deliver(msg);
                    } else { // user is not online, store messages for him
                        acc->store_msg(msg);
                    }
                }
            }
            else {*/
                // if private
                auto &receivers = data->get_user_list();
                for (auto i = 0; i < receivers.size(); i++) {
                    auto uid = receivers[i];
                    AccountData *acc = db_->get_account(uid);
                    Session *s = acc->get_session();
                    if (s) {
                        s->deliver(msg);
                    } else { // user is not online, store messages for him
                        acc->store_msg(msg);
                    }
                }
            //}
        }

        void Server::do_accept() {
            auto acceptor_f = [this] (boost::system::error_code error_code) {
                if (!error_code)
                    std::make_shared<Session>(this, rooms_.back())->start();
                else
                    cerr << "Error Code in do_accept" << endl;
                do_accept();
            };

            // Connect to last and only chatroom for now
            acceptor_.async_accept(*(rooms_.back().get_chatroom_socket()), acceptor_f);
        }

    }
} // namespace meow


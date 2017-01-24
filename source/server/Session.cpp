#include <boost/asio.hpp>
#include <iostream>
#include <boost/algorithm/string.hpp>

#include "server_headers/AccountData.hpp"
#include "server_headers/Chatroom.hpp"
#include "server_headers/Server.hpp"
#include "lib_headers/Message.hpp"
#include "server_headers/Session.hpp"

namespace meow {
	namespace server {

		using boost::asio::ip::tcp;

		Session::Session(Server* server, Chatroom &room):
                server_(server),
                socket_(std::move(*room.get_chatroom_socket())),
				room_(room),
				msg_buf_()
		{
			msg_buf_.resize(SerializedMessage::MAX_MSG_LENGTH);
		}

		void Session::start() {
			room_.join(shared_from_this());
			do_read_header();
		}

		void Session::deliver(const SerializedMessage &msg) {
			bool write_in_progress = !write_msgs_.empty();
			write_msgs_.push_back(msg);
			if (!write_in_progress)
				do_write();
		}


		void Session::do_read_header() {
			auto read_header_f = [this](boost::system::error_code error_code, std::size_t /*length*/) {
				if (!error_code) {
					msg_buf_.decode_msg_length();
					do_read_body();
				} else {
					//std::cout << "error code in session::do_read_header" << endl;
					room_.leave(shared_from_this());
				}
			};

			boost::asio::async_read(socket_,
									boost::asio::buffer(msg_buf_.get_buf(),
														SerializedMessage::HEADER_LENGTH),
									read_header_f
			);
		}

		void Session::do_read_body() {
			auto read_body_f = [this](boost::system::error_code error_code, std::size_t /*length*/) {
				if (!error_code) {
					Message msg(msg_buf_);
					//cout << msg << endl;
					if (msg.get_msg_type() == Message::MsgType::LOGIN) {
                        vector<string> argv;
                        boost::split(argv, msg.get_msg_body(), boost::is_any_of("\t "));

                        AccountData new_acc(argv[0], AccountData::str_to_md5(argv[1]));
                        ServerDatabase* db = server_->get_db();
                        AccountData* acc = db->get_account(new_acc.get_user_id());
                        if (!acc) { // no user with such a nickname
                            new_acc.set_online(true);
                            db->add_account(new_acc);
                            Message response(Message::MsgType::LOGIN, "Account successfully created",
                                             0, new_acc.get_user_id());
                            cout << "create new account" << endl;
                            this->deliver(response);
                        }
                        // account was found. check password
                        else if (!acc->check_passwd(argv[1])) {
                            cout << "wrong password" << endl;
                            this->deliver(Message(Message::MsgType::LOGIN, "Wrong password", 0, 0));
                        }
                        // all is OK
                        else {
                            cout << "successfully logged into old account" << endl;
                            acc->set_online(true);
                            Message response(Message::MsgType::LOGIN,
                                             "Logged in succesfully", 0, new_acc.get_user_id());
                            this->deliver(response);
                        }
					}
					else if (msg.get_msg_type() == Message::MsgType::UID_REQUEST) {
						AccountData* acc = server_->get_db()->get_account(msg.get_msg_body());
						if (acc) {
                            ostringstream resp;
                            resp << acc->get_user_id();
                            Message response(Message::MsgType::UID_REQUEST,
                                             resp.str(), 0, msg.get_from_uid());
                            this->deliver(response);
						}
                        else {
                            Message response(Message::MsgType::ERROR,
                                             "No user with nick " + msg.get_msg_body() + " found",
                                             0, msg.get_from_uid());
                            this->deliver(response);
                        }
					}
                    else if (msg.get_msg_type() == Message::MsgType::NEWROOM) {
                        // extract uids from message body
                        istringstream iss(msg.get_msg_body());
                        int n_users;
                        Message::uid_t id;
                        vector<Message::uid_t> uids;
                        iss >> n_users;
                        for (int i = 0; i < n_users; i++) {
                            iss >> id;
                            uids.push_back(id);
                        }

                        cout << "create new room for " << msg.get_msg_body() << endl;

                        ChatroomData* room = server_->get_db()->get_room(uids);
                        if (room) {
                            cout << "created new room with id = " << room->get_room_id() << endl;
                            ostringstream resp;
                            resp << room->get_room_id(); // send room_id to client
                            Message response(Message::MsgType::NEWROOM,
                                             resp.str(), 0, msg.get_from_uid());
                            this->deliver(response);
                        }
                        else {
                            Message response(Message::MsgType::ERROR,
											 "Unable to create chat room",
                                             0, msg.get_from_uid());
                            this->deliver(response);
                        }
                    }
                    else if (msg.get_msg_type() == Message::MsgType::LOGOUT) {
                        AccountData* acc = server_->get_db()->get_account(msg.get_from_uid());
                        if (acc) {
                            acc->set_online(false);
                            cout << "Log out " << msg.get_from_uid() << endl;
                        }
                    }
					else { // ordinary text
						room_.deliver(msg);
					}
					do_read_header();
				} else {
					//std::cout << "error code in session::do_read_body" << endl;
					room_.leave(shared_from_this());
				}
			};

			boost::asio::async_read(socket_,
									boost::asio::buffer(msg_buf_.get_body_buf(),
														msg_buf_.get_body_len()),
									read_body_f
			);
		}

		void Session::do_write() {
			auto write_f = [this](boost::system::error_code error_code, std::size_t /*length*/) {
				if (!error_code) {
					write_msgs_.pop_front();
					if (!write_msgs_.empty())
						do_write();
				} else {
					std::cout << "error code in session::do_write" << endl;
					room_.leave(shared_from_this());
				}
			};

			SerializedMessage msg_buf = write_msgs_.front();

			boost::asio::async_write(socket_,
									 boost::asio::buffer(msg_buf.get_buf(),
														 msg_buf.get_msg_len()),
									 write_f);
		}

	}
} // namespace meow


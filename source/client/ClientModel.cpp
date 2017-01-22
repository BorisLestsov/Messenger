#include <iostream>

#include "client_headers/ClientModel.hpp"

namespace meow {
    namespace client {
		
		ClientModel::ClientModel()
            :   user_id_(0),
                user_nick_("")
		{
			
		}

        ClientModel::~ClientModel()
        {

        }
		
		void ClientModel::add_observer(Observer* observer)
		{
			observers_.push_front(observer);
		}

		void ClientModel::add_message(const Message& msg)
        {
            dialog_.push_back(msg);
            notify_all();
        }

        deque<Message>* ClientModel::get_dialog()
        {
            return &dialog_;
        }

        ClientModel::uid_t ClientModel::get_user_id() const
        {
            return user_id_;
        }

        void ClientModel::set_user_id(ClientModel::uid_t new_id)
        {
            user_id_ = new_id;
        }

        bool ClientModel::has_user_id() const
        {
            return user_id_ != 0;
        }

        string ClientModel::get_user_nick() const
        {
            return user_nick_;
        }

        void ClientModel::set_user_nick(const string& new_nick)
        {
            user_nick_ = new_nick;
        }

        bool ClientModel::has_user_nick() const
        {
            return user_nick_ != "";
        }
        
        // private
        void ClientModel::notify_all()
        {
			for (auto obs : observers_)
				obs->update();
		}

    }
} // namespace meow

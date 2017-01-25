#include <algorithm>
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

        void ClientModel::remove_observer(Observer* observer)
        {
            observers_.remove(observer);
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

        void ClientModel::reset_error()
        {
            error_on_ = false;
        }

        void ClientModel::set_error(bool err)
        {
            error_on_ = err;
        }

        bool ClientModel::has_error() const
        {
            return error_on_;
        }

        void ClientModel::set_error_message(const string& msg)
        {
            error_on_ = true;
            err_message_ = msg;
        }

        string ClientModel::get_error_message() const
        {
            return err_message_;
        }

        bool ClientModel::received_response() const
        {
            return response_;
        }

        void ClientModel::set_received_response(bool received)
        {
            response_ = received;
        }

        std::string ClientModel::translate_uid(Message::uid_t id) const
        {
            auto p = nick_map_.find(id);
            if (p != nick_map_.end())
                return p->second;
            return "<?>";
        }

        void ClientModel::add_uid_nick_pair(Message::uid_t id, const std::string& nick)
        {
            nick_map_[id] = nick;
        }
        
        // private
        void ClientModel::notify_all()
        {
			for (auto obs : observers_)
				obs->update();
		}

    }
} // namespace meow

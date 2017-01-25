#ifndef _CLIENT_MODEL_HPP_INCLUDED
#define _CLIENT_MODEL_HPP_INCLUDED

#include <list>
#include <deque>
#include <map>

#include "Observer.hpp"
#include "lib_headers/Message.hpp"

namespace meow {
	namespace client {

		class ClientModel {
		public:
            typedef Message::uid_t uid_t;

			ClientModel();
            ~ClientModel();
			
			void add_observer(Observer*);
			void remove_observer(Observer*);
            void add_message(const Message&);
            std::deque<Message>* get_dialog();

            uid_t get_user_id() const;
            void set_user_id(uid_t new_id);
            bool has_user_id() const;
            string get_user_nick() const;
            void set_user_nick(const string& new_nick);
            bool has_user_nick() const;

			// errno-like mechanism
            void reset_error();
            void set_error(bool err);
            bool has_error() const;
            void set_error_message(const string& msg);
            string get_error_message() const;

            bool received_response() const;
            void set_received_response(bool received);

			std::string translate_uid(Message::uid_t id) const;
            void add_uid_nick_pair(Message::uid_t id, const std::string& nick);

        private:
            std::deque<Message> dialog_;
			std::list<Observer*> observers_;

            // account data
            uid_t user_id_;
            std::string user_nick_;

            // errors
            bool error_on_;
            std::string err_message_;

            bool response_;

            std::map<Message::uid_t, std::string> nick_map_;

			void notify_all();
		}; // class ClientModel

	}
} // namespace meow


#endif // _CLIENT_MODEL_HPP_INCLUDED

#ifndef _CLIENT_MODEL_HPP_INCLUDED
#define _CLIENT_MODEL_HPP_INCLUDED

#include <list>
#include <deque>

#include "Observer.hpp"
#include "lib_headers/Message.hpp"

namespace meow {
	namespace client {

		class ClientModel {
		public:
			ClientModel();
			
			void add_observer(Observer*);
            void add_message(const Message&);
            std::deque<Message>* get_dialog();
			~ClientModel();

		private:
			std::deque<Message> dialog_;
			std::list<Observer*> observers_;
			void notify_all();
		}; // class ClientModel

	}
} // namespace meow


#endif // _CLIENT_MODEL_HPP_INCLUDED

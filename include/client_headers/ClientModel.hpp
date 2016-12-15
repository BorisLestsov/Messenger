#ifndef _CLIENT_MODEL_HPP_INCLUDED
#define _CLIENT_MODEL_HPP_INCLUDED

#include <list>

#include "Observer.hpp"

namespace meow {
	namespace client {

		class ClientModel {
		public:
			ClientModel();
			
			void add_observer(Observer*);
			~ClientModel();

		private:
			std::list<Observer*> observers_;
			void notify_all();
		}; // class ClientModel

	}
} // namespace meow


#endif // _CLIENT_MODEL_HPP_INCLUDED

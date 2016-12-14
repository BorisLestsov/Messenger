#ifndef _CLIENT_MODEL_HPP_INCLUDED
#define _CLIENT_MODEL_HPP_INCLUDED

#include <deque>

#include "Observer.hpp"

namespace meow {

	class ClientModel {
	public:
		ClientModel();
		~ClientModel();
	private:
		std::deque<Observer*> observers;
	}; // class ClientModel

} // namespace meow


#endif // _CLIENT_MODEL_HPP_INCLUDED

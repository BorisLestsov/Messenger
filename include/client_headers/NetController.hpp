#ifndef _NET_CONTROLLER_HPP_INCLUDED
#define _NET_CONTROLLER_HPP_INCLUDED

#include "ClientModel.hpp"
#include "lib_headers/Message.hpp"

namespace meow {
	namespace client {

		class NetController {
		public:
			NetController(ClientModel* model);

			~NetController();

			void send(Message &msg);

		private:
			ClientModel* model_;
		}; // class NetController

	}
} // namespace meow


#endif // _NET_CONTROLLER_HPP_INCLUDED

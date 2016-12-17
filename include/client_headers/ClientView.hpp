#ifndef _CLIENT_VIEW_HPP_INCLUDED
#define _CLIENT_VIEW_HPP_INCLUDED

#include "ClientModel.hpp"
#include "NetController.hpp"
#include "Observer.hpp"

namespace meow {
	namespace client {

		class ClientView : public Observer {
		public:
			virtual ~ClientView();
			virtual void start() = 0;
			virtual void update() = 0;

		protected:
			ClientView(NetController*, ClientModel*);
            void send(const Message&);
            ClientModel* model_;

		private:
			NetController* controller_;
		}; // abstract class ClientView

	}
} // namespace meow


#endif // _CLIENT_VIEW_HPP_INCLUDED

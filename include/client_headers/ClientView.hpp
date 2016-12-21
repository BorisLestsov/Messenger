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

			ClientModel* get_model();
			NetController* get_controller();

		protected:
			ClientView(NetController*, ClientModel*);
            void send(const Message&);
            ClientModel* model_;

		private:
			NetController* controller_;
		}; // abstract class ClientView


        // inline methods

        inline ClientModel* ClientView::get_model()
        {
            return model_;
        }

        inline NetController* ClientView::get_controller()
        {
            return controller_;
        }
	}
} // namespace meow


#endif // _CLIENT_VIEW_HPP_INCLUDED

#ifndef _CLIENT_VIEW_HPP_INCLUDED
#define _CLIENT_VIEW_HPP_INCLUDED

#include "ClientModel.hpp"
#include "NetController.hpp"
#include "Observer.hpp"

namespace meow {
	namespace client {

		class ClientUI :
				public Observer {
		public:
			virtual ~ClientUI();
			virtual void start() = 0;
			virtual void update() = 0;

			ClientModel* get_model();
			NetController* get_controller();

		protected:
			ClientUI(NetController*, ClientModel*);
            void send(const Message&);
            ClientModel* model_;

		private:
			NetController* controller_;
		}; // abstract class ClientUI


        // inline methods

        inline ClientModel* ClientUI::get_model()
        {
            return model_;
        }

        inline NetController* ClientUI::get_controller()
        {
            return controller_;
        }
	}
} // namespace meow


#endif // _CLIENT_VIEW_HPP_INCLUDED

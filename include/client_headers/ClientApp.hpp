#ifndef _CLIENT_APP_HPP_INCLUDED
#define _CLIENT_APP_HPP_INCLUDED

#include "ClientModel.hpp"
#include "ClientView.hpp"
#include "NetController.hpp"

namespace meow {
	namespace client {

		class ClientApp {
		public:
			static const int CONSOLE_APP;
			static const int NCURSES_APP;
			static const int QT_APP;

			ClientApp();
			
			int main(int argc, char** argv);

			~ClientApp();

		private:
			NetController *controller_;
			ClientModel *model_;
			ClientView *view_;
		}; // class ClientApp

	}
} // namespace meow


#endif // _CLIENT_APP_HPP_INCLUDED


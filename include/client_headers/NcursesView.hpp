#ifndef _NCURSES_VIEW_HPP_INCLUDED
#define _NCURSES_VIEW_HPP_INCLUDED

#include "ClientModel.hpp"
#include "ClientView.hpp"
#include "NetController.hpp"
#include "NcursesInputWindow.hpp"
#include "NcursesTerminal.hpp"
#include "lib_headers/ncurses-api.hpp"

namespace meow {
	namespace client {

		class NcursesView : public ClientView {
		public:
			NcursesView(NetController*, ClientModel*);

			void start();
			void update();

			~NcursesView();

        private:
            void refresh();

			NcursesTerminal* terminal_;
		}; // class NcursesView

	} // namespace client
} // namespace meow


#endif // _CLIENT_VIEW_HPP_INCLUDED


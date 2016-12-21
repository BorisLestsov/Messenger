#ifndef _NCURSES_VIEW_HPP_INCLUDED
#define _NCURSES_VIEW_HPP_INCLUDED

#include "ClientModel.hpp"
#include "ClientUI.hpp"
#include "NetController.hpp"
#include "CursesInputWindow.hpp"
#include "CursesTerminal.hpp"
#include "lib_headers/ncurses-api.hpp"

namespace meow {
	namespace client {

		class CursesUI : public ClientUI {
		public:
			CursesUI(NetController*, ClientModel*);

			void start();
			void update();

			~CursesUI();

        private:
            void refresh();

			CursesTerminal* terminal_;
		}; // class CursesUI

	} // namespace client
} // namespace meow


#endif // _CLIENT_VIEW_HPP_INCLUDED


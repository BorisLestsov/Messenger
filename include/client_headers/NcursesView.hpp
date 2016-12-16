#ifndef _NCURSES_VIEW_HPP_INCLUDED
#define _NCURSES_VIEW_HPP_INCLUDED


#include <ncurses.h>
// C++-style wrapper for ncurses utilities
namespace ncurses {
    using ::wgetch;   // note! ncurses::getch in a macrodefinition
    using ::refresh;
    using ::initscr;
    using ::endwin;
    using ::printw;
}

#include "ClientModel.hpp"
#include "ClientView.hpp"
#include "NetController.hpp"

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

		}; // class NcursesView

	} // namespace client
} // namespace meow


#endif // _CLIENT_VIEW_HPP_INCLUDED


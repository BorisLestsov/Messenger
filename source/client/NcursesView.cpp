#include "client_headers/ClientModel.hpp"
#include "client_headers/ClientView.hpp"
#include "client_headers/NcursesView.hpp"
#include "client_headers/NetController.hpp"

namespace meow {
	namespace client {
		
		NcursesView::NcursesView(NetController* netctl, ClientModel* model)
			:	ClientView(netctl, model)
		{

		}

        void NcursesView::start()
        {
            ncurses::initscr();			/* Start curses mode 		  */
            ncurses::printw("Hello World !!!");	/* Print Hello World		  */
            refresh();			/* Print it on to the real screen */
            ncurses::getch();			/* Wait for user input */
        }

		void NcursesView::update()
		{
		}
			
		NcursesView::~NcursesView()
		{
            ncurses::endwin();			/* End curses mode		  */
		}

        // private methods
        void NcursesView::refresh()
        {
            ncurses::refresh();
        }

	} // namespace client
} // namespace meow

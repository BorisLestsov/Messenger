#include "lib_headers/ncurses-api.hpp"
#include "lib_headers/Message.hpp"
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
            int row, col;

            ncurses::initscr();			    // start curses mode
            ncurses::raw();				    // line buffering disabled
            ncurses::keypad(stdscr, TRUE);	// we get F1, F2 etc...
            ncurses::noecho();			    // controlled echoing
            getmaxyx(stdscr, row, col);

            printw("size = %d x %d", row, col);
            refresh();

            // create input subwindow
            int height = 5;
            int width = col;
            int starty = row-5;
            int startx = 0;
            ncurses::WINDOW* inp_win = newwin(height, width, starty, startx);
            box(inp_win, 0 , 0);

            mvwprintw(inp_win, 0, 3, " Please, type message and press ENTER to send: ");

            wrefresh(inp_win);		/* Show that box 		*/

            // create subwindow for received window
            height = row - height - 1;
            starty = 0;
            startx = 0;
            ncurses::WINDOW* chat_win = newwin(height, width, starty, startx);
            box(chat_win, 0 , 0);
            mvwprintw(chat_win, 0, col/2-3, " Chat ");
            wrefresh(chat_win);

            //mvprintw(row-2,0,"This screen has %d rows and %d columns\n",row,col);
            //refresh();			/* Print it on to the real screen */
            wmove(inp_win, 1, 1);
            while (true) {
                int c = ncurses::wgetch(inp_win);
                int x, y;
                Message msg;

                switch (c) {
                    case ncurses::KEY_CTRL_C:
                        delwin(inp_win);
                        return;
                    case ncurses::KEY_BACKSP:
                        getyx(inp_win, y, x);
                        if (x != 1) {
                            mvwaddch(inp_win, y, x - 1, ' ');  // remove previous symbol
                            wmove(inp_win, y, x - 1);
                        }
                        // else no symbols in input window
                        break;
                    case '\n':
                        // here: send message
                        msg = Message(Message::MsgType::TEXT, "hello, world!", 42, 69, 100500);
                        send(msg);
                        break;
                    default:
                        waddch(inp_win, c);
                        break;
                        //wprintw(inp_win, "%d", c);
                }
            }
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

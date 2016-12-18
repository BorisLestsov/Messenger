#include <iostream>
#include <deque>

#include "lib_headers/ncurses-api.hpp"
#include "lib_headers/Message.hpp"
#include "client_headers/ClientModel.hpp"
#include "client_headers/ClientView.hpp"
#include "client_headers/NcursesDialog.hpp"
#include "client_headers/NcursesView.hpp"
#include "client_headers/NetController.hpp"

namespace meow {
	namespace client {

        using std::deque;

		NcursesView::NcursesView(NetController* netctl, ClientModel* model)
			:	ClientView(netctl, model)
		{
            chat_win_ = nullptr;
            inp_win_  = nullptr;
		}

        void NcursesView::start()
        {
            int row, col;

            ncurses::initscr();			    // start curses mode
            ncurses::raw();				    // line buffering disabled
            ncurses::keypad(stdscr, TRUE);	// we get F1, F2 etc...
            ncurses::noecho();			    // controlled echoing
            getmaxyx(stdscr, row, col);

            //printw("size = %d x %d", row, col);
            refresh();

            // create input subwindow
            int height = 5;
            int width = col;
            int starty = row-5;
            int startx = 0;
            inp_win_ = new NcursesInputWindow(height, width, starty, startx);
            //ncurses::WINDOW* inp_win = newwin(height, width, starty, startx);

            //box(inp_win, 0 , 0);

            //mvwprintw(inp_win, 0, 3, " Please, type message and press ENTER to send: ");

            //wrefresh(inp_win);		/* Show that box 		*/

            // create subwindow for received window
            height = row - height - 1;
            starty = 0;
            startx = 0;
            chat_win_ = newwin(height, width, starty, startx);
            box(chat_win_, 0 , 0);
            mvwprintw(chat_win_, 0, col/2-3, " Chat ");
            wrefresh(chat_win_);

            //mvprintw(row-2,0,"This screen has %d rows and %d columns\n",row,col);
            //refresh();			/* Print it on to the real screen */
            while (true) {
                int c = inp_win_->input();
                if (c == ncurses::KEY_CTRL_C) {
                    NcursesDialog("Are you sure you want to exit?").ask_user();
                    return;
                }
                else if (c == '\n') {  // create Message and then send it!
                    Message msg = Message(Message::MsgType::TEXT, inp_win_->get_text(), 42, 69, 100500);
                    send(msg);
                    inp_win_->reset(); // clear input text area
                }
            }
        }

        // draw messages from other clients of this chat
		void NcursesView::update()
		{
            deque<Message>* dialog = model_->get_dialog();
            for (size_t i = 0; i < dialog->size(); i++) {
                mvwprintw(chat_win_, i+1, 2, "%s", dialog->at(i).get_msg_body().c_str());
            }
            wrefresh(chat_win_);
            if (inp_win_)
                inp_win_->focus();  // return focus to the input window
		}

		NcursesView::~NcursesView()
		{
            delete inp_win_;
            ncurses::endwin();			/* End curses mode		  */
		}

        // private methods
        void NcursesView::refresh()
        {
            ncurses::refresh();
        }

	} // namespace client
} // namespace meow

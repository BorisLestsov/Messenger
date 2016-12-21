#include <iostream>
#include <deque>

#include "lib_headers/ncurses-api.hpp"
#include "lib_headers/Message.hpp"
#include "client_headers/ClientModel.hpp"
#include "client_headers/NcursesTerminal.hpp"
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
            //chat_win_ = nullptr;
            //inp_win_  = nullptr;
		}

        void NcursesView::start()
        {
            int row, col;

            ncurses::initscr();			    // start curses mode
            ncurses::raw();				    // line buffering disabled
            ncurses::keypad(stdscr, TRUE);	// we get F1, F2 etc...
            ncurses::noecho();			    // controlled echoing

            // init ncurses color engine
            start_color();
            use_default_colors();
            ncurses::init_pair(ncurses::ColorPair::GREEN_BLACK, COLOR_GREEN,  COLOR_BLACK);
            ncurses::init_pair(ncurses::ColorPair::WHITE_BLUE,  COLOR_WHITE,  COLOR_BLUE);
            ncurses::init_pair(ncurses::ColorPair::YELLOW_BLUE, COLOR_YELLOW, COLOR_BLUE);

            getmaxyx(stdscr, row, col);

            //printw("size = %d x %d", row, col);
            refresh();

            terminal_ = new NcursesTerminal(this, row, col, 0, 0);
            terminal_->input();

            // create start subwindow
            /*int height = 5;
            int width = col;
            int starty = row-6;
            int startx = 0;
            inp_win_ = new NcursesInputWindow(height, width, starty, startx);

            // create subwindow for received messages
            height = row - height - 1;
            starty = 0;
            startx = 0;
            chat_win_ = newwin(height, width, starty, startx);
            box(chat_win_, 0 , 0);
            mvwprintw(chat_win_, 0, col/2-3, " Chat ");
            wrefresh(chat_win_);

            terminal_ = new NcursesTerminal(row, col, 0, 0);
            terminal_->start();*/

            //mvprintw(row-2,0,"This screen has %d rows and %d columns\n",row,col);
            //refresh();			/* Print it on to the real screen */
            /*while (true) {
                int c = inp_win_->input();
                if (c == ncurses::KEY_CTRL_C) {
                    NcursesDialog::Answer ans = NcursesDialog("Are you sure you want to exit?").ask_user();
                    if (ans == NcursesDialog::YES)
                        return;
                    refresh(); // ?
                    wrefresh(chat_win_);
                    inp_win_->refresh();
                    update();
                }
                else if (c == '\n') {  // create Message and then send it!
                    Message msg = Message(Message::MsgType::TEXT, inp_win_->get_text(), 42, 69, 100500);
                    send(msg);
                    inp_win_->reset(); // clear start text area
                }
                else if (c == ncurses::KEY_ESC) {
                    // go to Vim-like console

                }
            }*/
        }

        // draw messages from other clients of this chat
		void NcursesView::update()
		{
            deque<Message>* dialog = model_->get_dialog();
            /*for (size_t i = 0; i < dialog->size(); i++) {
                mvwprintw(chat_win_, i+1, 2, "%s", dialog->at(i).get_msg_body().c_str());
            }
            wrefresh(chat_win_);
            if (inp_win_)
                inp_win_->focus();  // return focus to the start window*/
		}

		NcursesView::~NcursesView()
		{
            //delete inp_win_;
            delete terminal_;
            ncurses::endwin();			// End curses mode
		}

        // private methods

        void NcursesView::refresh()
        {
            ncurses::refresh();
        }

	} // namespace client
} // namespace meow

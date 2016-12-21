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

        NcursesView::NcursesView(NetController *netctl, ClientModel *model)
                : ClientView(netctl, model) {
        }

        void NcursesView::start() {
            int row, col;

            ncurses::initscr();                // start curses mode
            ncurses::raw();                    // line buffering disabled
            ncurses::keypad(stdscr, TRUE);    // we get F1, F2 etc...
            ncurses::noecho();                // controlled echoing

            // init ncurses color engine
            start_color();
            use_default_colors();
            ncurses::init_pair(ncurses::ColorPair::GREEN_BLACK, COLOR_GREEN, COLOR_BLACK);
            ncurses::init_pair(ncurses::ColorPair::WHITE_BLUE, COLOR_WHITE, COLOR_BLUE);
            ncurses::init_pair(ncurses::ColorPair::YELLOW_BLUE, COLOR_YELLOW, COLOR_BLUE);
            ncurses::init_pair(ncurses::ColorPair::RED_BLACK, COLOR_RED, COLOR_BLACK);
            ncurses::init_pair(ncurses::ColorPair::BLUE_BLACK, COLOR_BLUE, COLOR_BLACK);

            getmaxyx(stdscr, row, col);

            terminal_ = new NcursesTerminal(this, row, col, 0, 0);
            terminal_->input();
        }

        // draw messages from other clients of this chat
        void NcursesView::update() {
        }

        NcursesView::~NcursesView()
        {
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

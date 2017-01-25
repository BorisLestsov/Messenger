#include <iostream>
#include <deque>

#include "lib_headers/ncurses-api.hpp"
#include "lib_headers/Message.hpp"
#include "client_headers/ClientModel.hpp"
#include "client_headers/CursesTerminal.hpp"
#include "client_headers/ClientUI.hpp"
#include "client_headers/CursesDialog.hpp"
#include "client_headers/CursesUI.hpp"
#include "client_headers/NetController.hpp"

namespace meow {
	namespace client {

        using std::deque;

        CursesUI::CursesUI(NetController *netctl, ClientModel *model)
                : ClientUI(netctl, model) {
            model_->add_observer(this);
        }

        void CursesUI::start() {
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
            ncurses::init_pair(ncurses::ColorPair::MAGENTA_BLACK, COLOR_MAGENTA, COLOR_BLACK);
            ncurses::init_pair(ncurses::ColorPair::YELLOW_BLACK, COLOR_YELLOW, COLOR_BLACK);
            ncurses::init_pair(ncurses::ColorPair::WHITE_BLACK, COLOR_WHITE, COLOR_BLACK);

            getmaxyx(stdscr, row, col);

            terminal_ = new CursesTerminal(this, row, col, 0, 0);
            terminal_->input();
        }

        // draw messages from other clients of this chat
        void CursesUI::update() {

        }

        CursesUI::~CursesUI()
        {
            delete terminal_;
            ncurses::endwin();			// End curses mode
            // remove observer -> bug!
		}

        // private methods

        void CursesUI::refresh()
        {
            ncurses::refresh();
        }

	} // namespace client
} // namespace meow

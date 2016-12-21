// C++-style wrapper for ncurses utilities

#ifndef _NCURSES_API_HPP_INCLUDED
#define _NCURSES_API_HPP_INCLUDED

#include <curses.h>

namespace ncurses {
    using ::box;
    using ::cbreak;
    using ::delwin;
    using ::echo;
    using ::endwin;
    using ::getmaxx;
    using ::getmaxy;
    using ::halfdelay;
    using ::init_pair;
    using ::initscr;
    using ::keypad;
    using ::mvwhline;
    using ::mvwprintw;
    using ::newwin;
    using ::noecho;
    using ::printw;
    using ::raw;
    using ::refresh;
    using ::waddch;
    using ::waddstr;
    using ::wattr_off;
    using ::wattr_on;
    using ::werase;
    using ::wgetch;   // note! ncurses::getch is a macrodefinition!
    using ::wprintw;
    using ::wrefresh;

    using ::WINDOW;

    const int KEY_CTRL_C = 3;
    const int KEY_NEWLINE = '\n';
    const int KEY_BACKSP = 127;
    const int KEY_ESC = 27;

    enum ColorPair {
        GREEN_BLACK = 3,
        WHITE_BLUE,
        YELLOW_BLUE,
        RED_BLACK,
        BLUE_BLACK
    };
}


#endif // _NCURSES_API_HPP_INCLUDED

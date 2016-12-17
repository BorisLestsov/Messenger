// C++-style wrapper for ncurses utilities

#ifndef _NCURSES_API_HPP_INCLUDED
#define _NCURSES_API_HPP_INCLUDED

#include <ncurses.h>

namespace ncurses {
    using ::box;
    using ::cbreak;
    using ::echo;
    using ::endwin;
    using ::getmaxx;
    using ::getmaxy;
    using ::halfdelay;
    using ::initscr;
    using ::keypad;
    using ::mvwprintw;
    using ::newwin;
    using ::noecho;
    using ::printw;
    using ::raw;
    using ::refresh;
    using ::waddch;
    using ::waddstr;
    using ::wgetch;   // note! ncurses::getch is a macrodefinition!
    using ::wprintw;
    using ::wrefresh;

    using ::WINDOW;

    const int KEY_CTRL_C = 3;
    const int KEY_NEWLINE = '\n';
    const int KEY_BACKSP = 127;
    /*
    const int KEY_ARROW_LEFT  = 37;
    const int KEY_ARROW_UP    = 65;
    const int KEY_ARROW_RIGHT = 39;
    const int KEY_ARROW_DOWN  = 66;*/
}


#endif // _NCURSES_API_HPP_INCLUDED

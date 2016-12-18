#ifndef _NCURSES_TERMINAL_HPP_INCLUDED
#define _NCURSES_TERMINAL_HPP_INCLUDED

#include "lib_headers/ncurses-api.hpp"

namespace meow {
    namespace client {

        class NcursesTerminal {
        public:
            NcursesTerminal();
            ~NcursesTerminal();
        private:
            ncurses::WINDOW* self_;
        }; // class NcursesTerminal

    } // namespace client
} // namespace meow

#endif // _NCURSES_TERMINAL_HPP_INCLUDED

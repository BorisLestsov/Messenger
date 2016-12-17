#ifndef _NCURSES_INPUT_WINDOW_HPP_INCLUDED
#define _NCURSES_INPUT_WINDOW_HPP_INCLUDED

#include <string>

#include "lib_headers/ncurses-api.hpp"

namespace meow {
    namespace client {

        class NcursesInputWindow {
        public:
            NcursesInputWindow(int w, int h, int y0, int x0);
            ~NcursesInputWindow();

            int  focus();
            void refresh();
            std::string get_text();
        private:
            ncurses::WINDOW* self_;
        };

    }
}

#endif // _NCURSES_INPUT_WINDOW_HPP_INCLUDED
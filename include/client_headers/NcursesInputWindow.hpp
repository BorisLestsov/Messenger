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

            int  input();
            void focus();
            void reset();
            void refresh();
            std::string get_text();
        private:
            ncurses::WINDOW* self_;
            // parameters of text area
            int ncol_, nrow_;
            int x0_, y0_;
            // current cursor position
            int x_, y_;
            // editable text
            std::string text_;

            void clear();
            void draw_text();
        };

    }
}

#endif // _NCURSES_INPUT_WINDOW_HPP_INCLUDED
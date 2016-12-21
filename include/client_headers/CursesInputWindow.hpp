#ifndef _NCURSES_INPUT_WINDOW_HPP_INCLUDED
#define _NCURSES_INPUT_WINDOW_HPP_INCLUDED

#include <string>

#include "lib_headers/ncurses-api.hpp"
#include "client_headers/CursesComponent.hpp"

namespace meow {
    namespace client {

        class CursesInputWindow : public CursesComponent {
        public:
            CursesInputWindow(int w, int h, int y0, int x0);
            ~CursesInputWindow();

            int  input();
            void focus();
            void reset();
            void refresh();
            std::string get_text();
        private:
            // parameters of text area
            int ncol_, nrow_;
            int x0_, y0_;
            // current cursor position
            int x_, y_;
            // editable text
            std::string text_;

            void draw_text();
        };

    }
}

#endif // _NCURSES_INPUT_WINDOW_HPP_INCLUDED
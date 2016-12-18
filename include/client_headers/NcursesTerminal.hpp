#ifndef _NCURSES_TERMINAL_HPP_INCLUDED
#define _NCURSES_TERMINAL_HPP_INCLUDED

#include <deque>

#include "lib_headers/ncurses-api.hpp"

namespace meow {
    namespace client {

        class NcursesTerminal {
        public:
            NcursesTerminal(int height, int width, int starty, int startx);
            ~NcursesTerminal();

            int get_width() const;
            int get_height() const;

            void refresh();

        private:
            ncurses::WINDOW* self_;
            int width_;
            int height_;

            struct output_line {
                time_t time_;
                std::string text_;

                output_line(std::string s);
                std::string date_to_s();
            };
            std::deque<output_line> out_buf_;
        }; // class NcursesTerminal

        inline int NcursesTerminal::get_width() const
        {
            return width_;
        }

        inline int NcursesTerminal::get_height() const
        {
            return height_;
        }

    } // namespace client
} // namespace meow

#endif // _NCURSES_TERMINAL_HPP_INCLUDED

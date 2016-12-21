#ifndef _CURSES_COMPONENT_HPP_INCLUDED
#define _CURSES_COMPONENT_HPP_INCLUDED

#include "lib_headers/ncurses-api.hpp"

namespace meow {
    namespace client {

        class CursesComponent {
        public:
            virtual ~CursesComponent();

            int get_width() const;
            int get_height() const;

            virtual void refresh() = 0;
        protected:
            CursesComponent(int height, int width, int starty, int startx);

            int width_;
            int height_;
            ncurses::WINDOW* self_;
        };

        inline int CursesComponent::get_width() const
        {
            return width_;
        }

        inline int CursesComponent::get_height() const
        {
            return height_;
        }

    } // namespace meow::client
} // namespace meow

#endif // _CURSES_COMPONENT_HPP_INCLUDED

#ifndef _NCURSES_DIALOG_HPP_INCLUDED
#define _NCURSES_DIALOG_HPP_INCLUDED

#include <string>

#include "lib_headers/ncurses-api.hpp"

namespace meow {
    namespace client {

        class CursesDialog {
        public:
            enum Answer { YES, NO };

            CursesDialog(const std::string&);
            ~CursesDialog();

            Answer ask_user();

            int get_width() const;
            int get_height() const;

        private:
            int width_;
            int height_;
            ncurses::WINDOW* self_;

            void refresh();
            void fill_background();
            void draw_string(const std::string&);
            void draw_buttons(Answer onhover);
        }; // end of class CursesDialog

        inline int CursesDialog::get_height() const
        {
            return height_;
        }

        inline int CursesDialog::get_width() const
        {
            return width_;
        }

        inline void CursesDialog::refresh()
        {
            wrefresh(self_);
        }

    } // namespace client
} // namespace meow

#endif  // _NCURSES_DIALOG_HPP_INCLUDED

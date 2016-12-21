#ifndef _NCURSES_DIALOG_HPP_INCLUDED
#define _NCURSES_DIALOG_HPP_INCLUDED

#include <string>

#include "lib_headers/ncurses-api.hpp"

namespace meow {
    namespace client {

        class NcursesDialog {
        public:
            enum Answer { YES, NO };

            NcursesDialog(const std::string&);
            ~NcursesDialog();

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
        }; // end of class NcursesDialog

        inline int NcursesDialog::get_height() const
        {
            return height_;
        }

        inline int NcursesDialog::get_width() const
        {
            return width_;
        }

        inline void NcursesDialog::refresh()
        {
            wrefresh(self_);
        }

    } // namespace client
} // namespace meow

#endif  // _NCURSES_DIALOG_HPP_INCLUDED

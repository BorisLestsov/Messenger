#include <iostream>
#include <string>

#include "client_headers/NcursesInputWindow.hpp"
#include "lib_headers/ncurses-api.hpp"
#include "lib_headers/Message.hpp"

namespace meow {
    namespace client {

        using std::cout;
        using std::endl;
        using std::string;

        NcursesInputWindow::NcursesInputWindow(int h, int w, int y0, int x0)
            :   x0_(1), y0_(1),          // upper left corner
                ncol_(w-2), nrow_(h-2),  // text are size (in symbols)
                x_(x0_), y_(y0_),        // cursor pointer to upper left corner
                text_("")
        {
            self_ = ncurses::newwin(h, w, y0, x0);
            // draw titled border
            box(self_, 0, 0);
            mvwprintw(self_, 0, 3, " Please, type message and press ENTER to send: ");
            refresh();
        }

        int NcursesInputWindow::input()
        {
            wmove(self_, y0_, x0_);
            while (true) {
                int c = ncurses::wgetch(self_);
                int x, y;
                Message msg;

                switch (c) {
                    case ncurses::KEY_CTRL_C:
                        return c;
                    case ncurses::KEY_BACKSP: // remove 1 char
                        /*getyx(self_, y, x);
                        if (x != 1) {
                            mvwaddch(self_, y, x - 1, ' ');  // remove previous symbol
                            wmove(self_, y, x - 1);
                        }*/
                        // if no symbols in the input window
                        if (x_ == x0_ && y_ == y0_)
                            ;
                        else {
                            int at = (y_-1)*ncol_ + (x_-1) - 1;
                            text_.erase(at, 1);
                            draw_text();
                            if (x_ > x0_)
                                x_--;
                            else if (y_ == y0_)
                                ;
                            else {  // rm first char in line
                                x_ = x0_ + ncol_ - 1;
                                y_--;
                            }
                        }
                        wmove(self_, y_, x_);
                        break;
                    /*case KEY_UP:
                        if (y_ == y0_)
                            return KEY_UP;
                        y_--;
                        wmove(self_, y_, x_);
                        break;
                    case KEY_DOWN:
                        if (y_ != y0_ + nrow_)  // if not last row
                            y_++;
                        wmove(self_, y_, x_);
                        break;*/
                    case '\n':
                        // send message & reset input window
                        return '\n';
                    default:  // insert one char
                        // no more text than nrow_ lines is allowed now!
                        if (y_ >= y0_+nrow_)
                            break;
                        int at = (y_-1)*ncol_ + (x_-1);
                        text_.insert(at, 1, c);
                        draw_text();
                        if (x_ < ncol_)
                            x_++;
                        else { // to the next line
                            x_ = x0_;
                            y_++;
                        }
                        wmove(self_, y_, x_);
                        break;
                        //wprintw(inp_win, "%d", c);
                }
            }
        }

        void NcursesInputWindow::reset()
        {
            clear();
            text_ = "";
            x_ = x0_;
            y_ = y0_;
            wmove(self_, y_, x_);
            refresh();
        }

        void NcursesInputWindow::focus()
        {
            wmove(self_, y_, x_);
            refresh();
        }

        string NcursesInputWindow::get_text()
        {
            return text_;
        }

        void NcursesInputWindow::refresh()
        {
            wrefresh(self_);
        }

        NcursesInputWindow::~NcursesInputWindow()
        {
            delwin(self_);
        }

        // private methods

        void NcursesInputWindow::clear()
        {
            for (int i = y0_; i < y0_+nrow_; i++)
                for (int j = x0_; j < x0_ + ncol_; j++)
                    mvwaddch(self_, i, j, ' ');
        }

        void NcursesInputWindow::draw_text()
        {
            clear();
            for (size_t i = 0; i < text_.length(); i += ncol_) {
                mvwprintw(self_, i/ncol_+1, x0_, "%s", text_.substr(i, ncol_).c_str());
            }
            refresh();
        }

    } // namespace meow::client
} // namespace meow
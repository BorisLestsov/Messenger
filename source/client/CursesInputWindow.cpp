#include <iostream>
#include <string>

#include "client_headers/CursesInputWindow.hpp"
#include "lib_headers/ncurses-api.hpp"
#include "lib_headers/Message.hpp"

namespace meow {
    namespace client {

        using std::cout;
        using std::endl;
        using std::string;

        CursesInputWindow::CursesInputWindow(int h, int w, int y0, int x0)
            :   CursesComponent(h, w, y0, x0),
                x0_(0), y0_(1),          // upper left corner
                ncol_(w), nrow_(h-1),    // text area size (in symbols)
                x_(x0_), y_(y0_),        // cursor pointer to upper left corner
                text_("")
        {
            ncurses::keypad(self_, TRUE);  // keyboard capture
            refresh();
        }

        int CursesInputWindow::input()
        {
            wmove(self_, y0_, x0_);
            while (true) {
                int c = ncurses::wgetch(self_);
                Message msg;

                switch (c) {
                    case ncurses::KEY_CTRL_C:
                        return c;
                    case ncurses::KEY_BACKSP: // remove 1 char
                        // if no symbols in the start window
                        if (x_ == x0_ && y_ == y0_)
                            ;
                        else {
                            int at = (y_-y0_)*ncol_ + x_ - 1;
                            text_.erase(at, 1);

                            if (x_ > x0_)
                                x_--;
                            else if (y_ == y0_)  // cursor at (x0_,y0_)
                                ;
                            else {  // rm first char in line
                                x_ = x0_ + ncol_ - 1;
                                y_--;
                            }
                        }
                        refresh();
                        break;
                    case KEY_LEFT:
                        if (x_ > x0_)
                            x_--;
                        wmove(self_, y_, x_);
                        break;
                    case KEY_RIGHT:
                        if (x_ < x0_+text_.length())
                            x_++;
                        wmove(self_, y_, x_);
                        break;
                    case KEY_DOWN:
                    case KEY_UP:
                        return c;
                    case ncurses::KEY_ESC:
                        return c;
                    case '\n':
                        // send message & reset start window
                        return '\n';
                    default:  // insert one char
                        // no more text than nrow_ lines is allowed now!
                        if (y_ >= y0_+nrow_)
                            break;
                        int at = (y_-y0_)*ncol_ + x_;
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
                }
            }
        }

        void CursesInputWindow::reset()
        {
            text_ = "";
            x_ = x0_;
            y_ = y0_;
            refresh();
        }

        void CursesInputWindow::focus()
        {
            wmove(self_, y_, x_);
            refresh();
        }

        string CursesInputWindow::get_text()
        {
            return text_;
        }

        void CursesInputWindow::refresh()
        {
            int width = ncol_ + 2;
            werase(self_);

            // blue header
            wattron(self_, COLOR_PAIR(ncurses::ColorPair::WHITE_BLUE));
            wattron(self_, A_BOLD);
            mvwhline(self_, 0, 0, ' ', width);
            mvwprintw(self_, 0, 0, " Please, type message and press ENTER to send: ");
            wattroff(self_, COLOR_PAIR(ncurses::ColorPair::WHITE_BLUE));
            wattroff(self_, A_BOLD);

            draw_text();
            wmove(self_, y_, x_);
            wrefresh(self_);
        }

        CursesInputWindow::~CursesInputWindow()
        {
        }

        // private methods

        void CursesInputWindow::draw_text()
        {
            for (size_t i = 0; i < text_.length(); i += ncol_) {
                mvwprintw(self_, y0_+i/ncol_, x0_, "%s", text_.substr(i, ncol_).c_str());
            }
        }

    } // namespace meow::client
} // namespace meow

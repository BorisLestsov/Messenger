#include <string>

#include "client_headers/NcursesInputWindow.hpp"
#include "lib_headers/ncurses-api.hpp"
#include "lib_headers/Message.hpp"

namespace meow {
    namespace client {

        using std::string;

        NcursesInputWindow::NcursesInputWindow(int w, int h, int y0, int x0)
        {
            self_ = ncurses::newwin(w, h, y0, x0);
            // draw titled border
            box(self_, 0, 0);
            mvwprintw(self_, 0, 3, " Please, type message and press ENTER to send: ");
            refresh();
        }

        int NcursesInputWindow::focus()
        {
            wmove(self_, 1, 1);
            while (true) {
                int c = ncurses::wgetch(self_);
                int x, y;
                Message msg;

                switch (c) {
                    case ncurses::KEY_CTRL_C:
                        return c;
                    case ncurses::KEY_BACKSP:
                        getyx(self_, y, x);
                        if (x != 1) {
                            mvwaddch(self_, y, x - 1, ' ');  // remove previous symbol
                            wmove(self_, y, x - 1);
                        }
                        // else no symbols in input window
                        break;
                    case '\n':
                        // here: send message
                        //msg = Message(Message::MsgType::TEXT, "hello, world!", 42, 69, 100500);
                        //send(msg);
                        return '\n';
                        //break;
                    default:
                        waddch(self_, c);
                        break;
                        //wprintw(inp_win, "%d", c);
                }
            }
        }

        string NcursesInputWindow::get_text()
        {
            return "hello, manul cats!";
        }

        void NcursesInputWindow::refresh()
        {
            wrefresh(self_);
        }

        NcursesInputWindow::~NcursesInputWindow()
        {
            delwin(self_);
        }

    } // namespace meow::client
} // namespace meow
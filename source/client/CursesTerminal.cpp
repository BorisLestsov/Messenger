#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "lib_headers/ncurses-api.hpp"
#include "client_headers/CursesChat.hpp"
#include "client_headers/CursesDialog.hpp"
#include "client_headers/CursesTerminal.hpp"
#include "client_headers/CursesUI.hpp"

namespace meow {
    namespace client {

        using std::ostringstream;
        using std::string;
        using std::vector;

        const int CursesTerminal::TIME_COL_WIDTH = 12;

        // class output_line helps to associate terminal output with current time
        CursesTerminal::output_line::output_line(const string& s)
            :   text_(s)
        {
            time(&time_);
        }

        string CursesTerminal::output_line::date_to_s()
        {
            static const char MAX_BUF = 80;
            char buffer[MAX_BUF];

            struct tm* timeinfo = localtime(&time_);
            strftime(buffer, MAX_BUF, "%H:%M:%S", timeinfo);
            return string(buffer);
        }

        // class CursesTerminal

        CursesTerminal::CursesTerminal(ClientUI* parent, int height, int width, int starty, int startx)
            :   parent_(parent),
                width_(width),
                height_(height),
                pos_(2)
        {
            self_ = newwin(height, width, startx, starty);
            keypad(self_, TRUE);

            // print logo: meow
            // $ figlet -k meow
            string logo = " _ __ ___    ___   ___ __      __\n"
                    "| '_ ` _ \\  / _ \\ / _ \\\\ \\ /\\ / /\n"
                    "| | | | | ||  __/| (_) |\\ V  V / \n"
                    "|_| |_| |_| \\___| \\___/  \\_/\\_/  \n\n";
            string short_info = string("Meow Messenger v0.2 build ") + __DATE__ + __TIME__ + "\n";
            out_buf_.push_front(output_line(short_info));

            this->refresh();
        }

        CursesTerminal::~CursesTerminal()
        {
            delwin(self_);
        }

        void CursesTerminal::refresh()
        {
            int x0 = 0, y0 = 0;

            werase(self_);

            // draw header and prompt blue lines
            wattron(self_, COLOR_PAIR(ncurses::ColorPair::WHITE_BLUE));
            wattron(self_, A_BOLD);

            // blue title line
            mvwhline(self_, y0, x0, ' ', width_);
            mvwprintw(self_, y0, x0, "Meow Messenger v0.2 2016-2017 https://github.com/BorisLestsov/Messenger");

            // blue prompt line
            mvwhline(self_, height_-2, x0, ' ', width_);
            mvwprintw(self_, height_-2, x0, "Type command and press ENTER:");

            wattroff(self_, COLOR_PAIR(ncurses::ColorPair::WHITE_BLUE));
            wattroff(self_, A_BOLD);

            draw_output_panel();
            draw_input_line("");

            ncurses::wrefresh(self_);
        }

        void CursesTerminal::input()
        {
            string cmd;
            int retval;
            CursesDialog::Answer ans;

            while (true) {
                int c = wgetch(self_);
                switch (c) {
                    case '\n':  // execute command
                        retval = exec(cmd);
                        if (retval == 1) { // fatal error or exit code
                            ans = CursesDialog("Are you sure you want to exit?").ask_user();
                            if (ans == CursesDialog::YES)
                                return;
                        }
                        // draw_input_line("");
                        refresh();
                        cmd = "";
                        pos_ = 2;
                        //wmove(self_, height_-1, 2);
                        break;
                    case ncurses::KEY_BACKSP: // backspace
                        if (!cmd.empty()) {
                            cmd.pop_back();  // C++11 required!
                            draw_input_line(cmd);
                            pos_--;
                        }
                        break;
                    case ncurses::KEY_CTRL_C:
                        ans = CursesDialog("Are you sure you want to exit?").ask_user();
                        if (ans == CursesDialog::YES)
                            return;
                        refresh();
                        break;
                    default:
                        if (pos_ >= width_)  // too many chars in start line
                            break;
                        cmd.insert(pos_-2, 1, c);
                        draw_input_line(cmd);
                        pos_++;
                        break;
                }
            }
        }

        int CursesTerminal::exec(const string& cmd)
        {
            if (cmd == "")
                out_buf_.push_front(output_line(""));
            else if (cmd == "help") {
                ostringstream msg;
                msg << "help          print this message\n";
                msg << "credits       print info about authors\n";
                msg << "about         print info about program\n";
                msg << "clear         remove all output\n";
                msg << "chat [room]   enter chat (global if no room is specified)\n";
                msg << "quit | exit   quit Meow Messenger";

                out_buf_.push_front(output_line(msg.str()));
            }
            else if (cmd == "credits") {
                string msg = "Contributors (in alphabetic order):\nBogovsky A., Lestsov B., Oleynichenko A., Sinukhov A.";
                out_buf_.push_front(output_line(msg));
            }
            else if (cmd == "about") {
                string msg = "Meow Messenger: lightweight TCP-based chat with command-line interface. ";
                msg += "For source code, see https://github.com/BorisLestsov/Messenger";
                out_buf_.push_front(output_line(msg));
            }
            else if (cmd == "clear") {
                out_buf_.clear();
                refresh();
            }
            else if (cmd == "chat") {
                CursesChat(parent_->get_controller(), parent_->get_model(),height_, width_, 0, 0)
                        .start();
            }
            else if (cmd == "quit" || cmd == "exit")
                return 1;
            else {   // error
                string msg = "Error! Type 'help' to see available options";
                out_buf_.push_front(output_line(msg));
            }
            draw_output_panel();
            return 0;
        }

        void CursesTerminal::draw_input_line(const std::string& text)
        {
            mvwhline(self_, height_-1, 0, ' ', width_);
            mvwprintw(self_, height_-1, 0, "> %s", text.c_str());
        }

        void CursesTerminal::draw_output_panel()
        {
            // 1. clear area
            for (int i = 1; i < height_-2; i++)
                mvwhline(self_, i, 0, ' ', width_);

            // 2. split every line and draw it substring-by-substring
            // TODO: refactor this code!
            int max_len_line = width_-TIME_COL_WIDTH-2;
            int y = height_-3;
            for (auto i = out_buf_.begin(); i != out_buf_.end() && y >= 1; i++) {
                string s = i->text_;
                vector<string> substrs;
                substrs.push_back("");
                for (auto ci = 0; ci < s.length(); ci++) {
                    auto c = s[ci];
                    if (c == '\n') {
                        substrs.push_back("");
                        continue;
                    }
                    if (substrs.back().length() == max_len_line)
                        substrs.push_back("");
                    substrs.back() += c;
                }
                // draw substrings in reverse order
                for (auto j = substrs.rbegin(); j != substrs.rend() && y >= 1; j++) {
                    mvwprintw(self_, y, TIME_COL_WIDTH + 1, j->c_str());
                    y--;
                }

                // draw time of current output line
                if (y > 0) {
                    wattron(self_, COLOR_PAIR(ncurses::ColorPair::GREEN_BLACK));
                    mvwprintw(self_, y + 1, 1, i->date_to_s().c_str());
                    wattroff(self_, COLOR_PAIR(ncurses::ColorPair::GREEN_BLACK));
                }
            }

            // 3. vertical separator
            wattron(self_, COLOR_PAIR(ncurses::ColorPair::GREEN_BLACK));
            mvwvline(self_, 1, TIME_COL_WIDTH-1, ACS_VLINE, height_-3);
            wattroff(self_, COLOR_PAIR(ncurses::ColorPair::GREEN_BLACK));
        }

    } // namespace client
} // namespace meow

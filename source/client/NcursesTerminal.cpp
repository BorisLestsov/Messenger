#include <ctime>
#include <string>

#include "lib_headers/ncurses-api.hpp"
#include "client_headers/NcursesTerminal.hpp"

namespace meow {
    namespace client {

        using std::string;

        // class output_line helps to associate terminal output with current time
        NcursesTerminal::output_line::output_line(string s)
            :   text_(s)
        {
            time(&time_);
        }

        string NcursesTerminal::output_line::date_to_s()
        {
            static const char MAX_BUF = 80;
            char buffer[MAX_BUF];

            struct tm* timeinfo = localtime(&time_);
            strftime(buffer, MAX_BUF, "%I:%M:%S", timeinfo);
            return string(buffer);
        }

        NcursesTerminal::NcursesTerminal(int height, int width, int starty, int startx)
            :   width_(width),
                height_(height)
        {
            self_ = newwin(height, width, startx, starty);
            keypad(self_, TRUE);

            this->refresh();
            wgetch(self_);
        }

        NcursesTerminal::~NcursesTerminal()
        {
            delwin(self_);
        }

        void NcursesTerminal::refresh()
        {
            int x0 = 0, y0 = 0;

            werase(self_);

            // draw header and prompt blue lines
            init_pair(1, COLOR_WHITE, COLOR_BLUE);
            wattron(self_, COLOR_PAIR(1));
            wattron(self_, A_BOLD);

            // blue title line
            mvwhline(self_, y0, x0, ' ', width_);
            mvwprintw(self_, y0, x0, "Meow Messenger v0.2 2016-2017 https://github.com/BorisLestsov/Messenger");

            // blue prompt line
            mvwhline(self_, height_-2, x0, ' ', width_);
            mvwprintw(self_, height_-2, x0, "Type command and press ENTER:");



            ncurses::wrefresh(self_);
        }

    } // namespace client
} // namespace meow

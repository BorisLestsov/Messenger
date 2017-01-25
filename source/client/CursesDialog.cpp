#include <cstring>
#include <iostream>
#include <string>

#include "client_headers/CursesDialog.hpp"
#include "lib_headers/ncurses-api.hpp"

namespace meow {
    namespace client {

        using std::string;

        CursesDialog::CursesDialog(const string &text) {
            int maxy, maxx;
            getmaxyx(stdscr, maxy, maxx);
            int w = (maxx >= 80) ? 60 : maxx - 20;
            int h = 10;
            int x0 = maxx / 2 - w / 2;
            int y0 = maxy / 2 - h / 2;
            self_ = ncurses::newwin(h, w, y0, x0);
            height_ = h;
            width_ = w;

            ncurses::keypad(self_, TRUE);

            // I want dialog windows to be colored and have bold font
            //start_color()
            init_pair(1, COLOR_YELLOW, COLOR_BLUE);
            wattron(self_, COLOR_PAIR(1));
            wattron(self_, A_BOLD);

            // draw titled border and question text
            box(self_, 0, 0);
            fill_background();
            mvwprintw(self_, 0, width_ / 2 - 1, " ? ");
            draw_string(text);

            // draw YES and NO buttons
            draw_buttons(NO);

            // return to default color scheme
            wattroff(self_, A_BOLD);
            wattroff(self_, COLOR_PAIR(1));
            refresh();
        }

        CursesDialog::~CursesDialog()
        {

            //init_pair(1, COLOR_WHITE, COLOR_BLACK);
            //wattron(self_, COLOR_PAIR(1));
            werase(self_);
            wrefresh(self_);
            delwin(self_);
        }

        CursesDialog::Answer CursesDialog::ask_user() {
            Answer ans = NO;
            while (true) {
                int c = wgetch(self_);
                if (c == KEY_LEFT)
                    draw_buttons(ans = YES);
                else if (c == KEY_RIGHT)
                    draw_buttons(ans = NO);
                else if (c == '\n')
                    return ans;
                else
                    return NO;  // default
            }
        }

        // private methods

        // fills all space inside the box with background color
        void CursesDialog::fill_background() {
            for (int i = 1; i < width_-1; i++)
                for (int j = 1; j < height_-1; j++)
                    mvwaddch(self_, j, i, ' ');
        }

        // smart text drawing
        void CursesDialog::draw_string(const std::string& text)
        {
            int x0 = 3;
            int y0 = 2;
            int xmax = width_ - x0;
            int x = x0, y = y0;

            char* s = strdup(text.c_str());
            char* p = strtok(s, " ");
            while (p) {   // split string into words
                mvwprintw(self_, y, x, p);
                int len = strlen(p);
                if (x + len + 1 > xmax) {
                    x = x0;
                    y++;
                }
                else
                    x += len + 1;
                p = strtok(nullptr, " ");
            }
        }

        void CursesDialog::draw_buttons(Answer onhover)
        {
            int button_len = 12;
            int betw_but = 10;

            init_pair(1, COLOR_YELLOW, COLOR_BLUE);
            init_pair(2, COLOR_YELLOW, COLOR_RED);
            wattron(self_, A_BOLD);

            int no_x = width_/2 + betw_but/2;
            int no_y = height_ - 3;
            short no_col = (onhover == NO) ? 2 : 1;
            wattron(self_, COLOR_PAIR(no_col));
            mvwprintw(self_, no_y, no_x, "     No     ");
            wattroff(self_, COLOR_PAIR(no_col));

            int yes_x = width_/2 - betw_but/2 - button_len;
            int yes_y = height_ - 3;
            short yes_col = (onhover == YES) ? 2 : 1;
            wattron(self_, COLOR_PAIR(yes_col));
            mvwprintw(self_, yes_y, yes_x, "    Yes     ");
            wattroff(self_, COLOR_PAIR(yes_col));
            wattroff(self_, A_BOLD);

            // select hovered button; cursor on the first letter
            wmove(self_, yes_y, onhover == YES ? yes_x+4 : no_x+5);
            refresh();
        }

    }
} // namespace meow

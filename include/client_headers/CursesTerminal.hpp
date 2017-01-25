#ifndef _NCURSES_TERMINAL_HPP_INCLUDED
#define _NCURSES_TERMINAL_HPP_INCLUDED

#include <deque>

#include "lib_headers/ncurses-api.hpp"
#include "client_headers/ClientUI.hpp"

namespace meow {
    namespace client {

        class CursesTerminal {
        public:
            CursesTerminal(ClientUI* parent, int height, int width, int starty, int startx);
            ~CursesTerminal();

            int get_width() const;
            int get_height() const;

            void refresh();
            void input();

        private:
            static const int TIME_COL_WIDTH;

            struct output_line {
                time_t time_;
                std::string text_;

                output_line(const std::string&);
                std::string date_to_s();
            };

            ClientUI* parent_;
            ncurses::WINDOW* self_;
            int width_;
            int height_;
            int pos_;    // current input position
            std::deque<output_line> out_buf_;
            // command history
            std::vector<std::string> hist_;
            int hist_pos_;

            int exec(const std::string &);
            void do_login(const string& nick_name, const string& passwd);
            void do_logout();
            Message::uid_t request_uid(const std::string& nick);
            Message::uid_t do_newroom(const std::vector<Message::uid_t>& user_ids);

            void draw_input_line(const std::string& text);
            void draw_output_panel();
        }; // class CursesTerminal

        inline int CursesTerminal::get_width() const
        {
            return width_;
        }

        inline int CursesTerminal::get_height() const
        {
            return height_;
        }

    } // namespace client
} // namespace meow

#endif // _NCURSES_TERMINAL_HPP_INCLUDED

#ifndef _NCURSES_CHAT_HPP_INCLUDED
#define _NCURSES_CHAT_HPP_INCLUDED

#include <deque>

#include "lib_headers/ncurses-api.hpp"
#include "lib_headers/Message.hpp"
#include "client_headers/NcursesInputWindow.hpp"
#include "client_headers/ClientView.hpp"

namespace meow {
    namespace client {
        class NcursesChat : public ClientView {
        public:
            NcursesChat(NetController* net, ClientModel* model, int height, int width, int starty, int startx);
            ~NcursesChat();

            int get_width() const;
            int get_height() const;

            void refresh();
            void start();
            void update();
        private:
            static const int INPUT_HEIGHT;

            ncurses::WINDOW* self_;
            ncurses::WINDOW* out_win_;
            NcursesInputWindow* inp_win_;
            int width_;
            int height_;

            void draw_msg_list();
        }; // class NcursesChat

        inline int NcursesChat::get_width() const
        {
            return width_;
        }

        inline int NcursesChat::get_height() const
        {
            return height_;
        }

    } // namespace meow::client
} // namespace meow

#endif // _NCURSES_CHAT_HPP_INCLUDED

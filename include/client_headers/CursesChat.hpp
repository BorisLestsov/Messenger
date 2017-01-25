#ifndef _NCURSES_CHAT_HPP_INCLUDED
#define _NCURSES_CHAT_HPP_INCLUDED

#include <deque>

#include "lib_headers/ncurses-api.hpp"
#include "lib_headers/Message.hpp"
#include "client_headers/CursesInputWindow.hpp"
#include "client_headers/ClientUI.hpp"

namespace meow {
    namespace client {
        class CursesChat : public ClientUI {
        public:
            CursesChat(NetController* net, ClientModel* model, Message::uid_t room_id,
                       int height, int width, int starty, int startx);
            ~CursesChat();

            int get_width() const;
            int get_height() const;

            void refresh();
            void start();
            void update();
        private:
            static const int INPUT_HEIGHT;

            ncurses::WINDOW* self_;
            ncurses::WINDOW* out_win_;
            CursesInputWindow* inp_win_;
            int width_;
            int height_;

            Message::uid_t room_id_;
            Message::uid_t my_id_;

            void draw_msg_list();
        }; // class CursesChat

        inline int CursesChat::get_width() const
        {
            return width_;
        }

        inline int CursesChat::get_height() const
        {
            return height_;
        }

    } // namespace meow::client
} // namespace meow

#endif // _NCURSES_CHAT_HPP_INCLUDED

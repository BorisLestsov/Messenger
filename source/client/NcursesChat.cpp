#include <deque>
#include <iostream>

#include "lib_headers/ncurses-api.hpp"
#include "lib_headers/Message.hpp"
#include "client_headers/NcursesChat.hpp"
#include "client_headers/NcursesDialog.hpp"

namespace meow {
    namespace client {

        using std::deque;

        const int NcursesChat::INPUT_HEIGHT = 3; // two borders are included: 1+3+1=5

        NcursesChat::NcursesChat(NetController* net, ClientModel* model,
                                 int height, int width, int starty, int startx)
            :   ClientView(net, model),
                width_(width),
                height_(height)
        {
            //model_->add_observer(this);
            self_ = newwin(height, width, startx, starty);

            // create start subwindow
            starty = height - INPUT_HEIGHT;
            startx = 0;
            inp_win_ = new NcursesInputWindow(INPUT_HEIGHT, width, starty, startx);

            // create subwindow for received messages
            starty = 0;
            startx = 0;
            out_win_ = newwin(height-INPUT_HEIGHT, width, starty, startx);

            this->refresh();
        }

        NcursesChat::~NcursesChat()
        {
            delete inp_win_;
            delwin(out_win_);
            delwin(self_);
        }

        void NcursesChat::update()
        {
            deque<Message>* dialog = model_->get_dialog();
            for (size_t i = 0; i < dialog->size(); i++) {
                mvwprintw(out_win_, i+1, 2, "%s %s", dialog->at(i).get_date("%H:%M:%S"), dialog->at(i).get_msg_body().c_str());
            }
            wrefresh(out_win_);
            inp_win_->refresh();
        }

        void NcursesChat::refresh()
        {
            draw_msg_list();
            inp_win_->refresh();
        }

        void NcursesChat::start()
        {
            int c;
            while ((c = inp_win_->input()) != ncurses::KEY_ESC) {
                if (c == ncurses::KEY_CTRL_C) {
                    /*NcursesDialog::Answer ans = NcursesDialog("Are you sure you want to exit?").ask_user();
                    if (ans == NcursesDialog::YES)
                        return;
                    refresh();*/
                    //update();
                    return;
                }
                else if (c == '\n') {  // create Message and then send it!
                    Message msg = Message(Message::MsgType::TEXT, inp_win_->get_text(), 42, 69, time(nullptr));
                    send(msg);
                    inp_win_->reset(); // clear start text area
                }
            }
        }


        // private methods

        void NcursesChat::draw_msg_list()
        {
            werase(out_win_);

            wattron(out_win_, COLOR_PAIR(ncurses::ColorPair::WHITE_BLUE));
            wattron(out_win_, A_BOLD);
            mvwhline(out_win_, 0, 0, ' ', width_);
            mvwprintw(out_win_, 0, 0, " Chat");
            string esc_tip = "ESC to exit chat";
            mvwprintw(out_win_, 0, width_-esc_tip.length(), esc_tip.c_str());
            wattroff(out_win_, COLOR_PAIR(ncurses::ColorPair::WHITE_BLUE));
            wattroff(out_win_, A_BOLD);

            wrefresh(out_win_);
        }

    } // namespace meow::client
} // namespace meow

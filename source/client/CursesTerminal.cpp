#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include <chrono>
#include <atomic>
#include <condition_variable>

using namespace std::chrono_literals;

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>

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
                pos_(2),
                hist_pos_(0)
        {
            self_ = newwin(height, width, startx, starty);
            keypad(self_, TRUE);

            // print logo: meow
            // $ figlet -k meow
            string logo = " _ __ ___    ___   ___ __      __\n"
                    "| '_ ` _ \\  / _ \\ / _ \\\\ \\ /\\ / /\n"
                    "| | | | | ||  __/| (_) |\\ V  V / \n"
                    "|_| |_| |_| \\___| \\___/  \\_/\\_/  \n\n";
            string short_info = string("Meow Messenger v0.2 build ") + __DATE__ + " " + __TIME__ + "\n";
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
                        hist_.push_back(cmd);
                        hist_pos_ = hist_.size(); // remember last command
                        retval = exec(cmd);
                        if (retval == 1) { // fatal error or exit code
                            ans = CursesDialog("Are you sure you want to exit?").ask_user();
                            if (ans == CursesDialog::YES) {
                                do_logout();
                                return;
                            }
                        }
                        // draw_input_line("");
                        refresh();
                        cmd = "";
                        pos_ = 2;
                        //wmove(self_, height_-1, 2);
                        break;
                    case ncurses::KEY_BACKSP: // backspace
                        if (!cmd.empty()) {
                            //cmd.pop_back();  // C++11 required!
                            cmd.erase(pos_-3, 1);
                            draw_input_line(cmd);
                            pos_--;
                            wmove(self_, height_-1, pos_);
                        }
                        break;
                    case KEY_UP:
                        if (hist_pos_-1 >= 0) {
                            hist_pos_--;
                            string new_cmd = hist_[hist_pos_];
                            draw_input_line(new_cmd);
                            cmd = new_cmd;
                            pos_ = new_cmd.length()+2;
                            wmove(self_, height_-1, pos_);
                        }
                        break;
                    case KEY_DOWN:
                        if (hist_pos_+1 < hist_.size()) {
                            hist_pos_++;
                            string new_cmd = hist_[hist_pos_];
                            draw_input_line(new_cmd);
                            cmd = new_cmd;
                            pos_ = new_cmd.length()+2;
                        }
                        else {
                            draw_input_line("");
                            cmd = "";
                            pos_ = 2;
                        }
                        break;
                    case KEY_LEFT:
                        if (pos_ > 2) {
                            pos_--;
                            wmove(self_, height_-1, pos_);
                        }
                        break;
                    case KEY_RIGHT:
                        if (pos_ < cmd.length()+2) {
                            pos_++;
                            wmove(self_, height_-1, pos_);
                        }
                        break;
                    case ncurses::KEY_CTRL_C:
                        ans = CursesDialog("Are you sure you want to exit?").ask_user();
                        if (ans == CursesDialog::YES) {
                            do_logout();
                            return;
                        }
                        refresh();
                        break;
                    default:
                        if (pos_ >= width_)  // too many chars in the command
                            break;
                        cmd.insert(pos_-2, 1, c);
                        draw_input_line(cmd);
                        pos_++;
                        wmove(self_, height_-1, pos_);
                        break;
                }
            }
        }

        int CursesTerminal::exec(const string& cmd)
        {
            std::vector<std::string> argv;
            boost::split(argv, cmd, boost::is_any_of("\t "));
            
            if (cmd == "")
                out_buf_.push_front(output_line(""));
            else if (cmd == "help") {
                ostringstream msg;
                msg << "help          print this message\n";
                msg << "credits       print info about authors\n";
                msg << "about         print info about program\n";
                msg << "clear         remove all output\n";
                msg << "login <nick>  login to your account or create a new one\n";
                msg << "logout        logout from account\n";
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
            else if (argv[0] == "chat") {
                if (argv.size() != 2) {
                    out_buf_.push_front(output_line("Error: wrong command! Format: chat [<nickname>]"));
                }
                else if (!parent_->get_model()->has_user_id()) {
                    out_buf_.push_front(output_line("Error: you are not logged in"));
                }
                    // global chat
                /*else if (argv.size() == 1) {
                    Message::uid_t room_id = do_newroom(vector<Message::uid_t>()); // empty vector
                    ostringstream oss;
                    oss << "room id global = " << room_id << endl;
                    out_buf_.push_front(output_line(oss.str()));
                    if (room_id) {   // here: room_id is the global chat identifier
                        CursesChat(parent_->get_controller(), parent_->get_model(), room_id,
                                   height_, width_, 0, 0)
                                .start();
                    }
                }*/
                    // private chat
                else {
                    Message::uid_t to = request_uid(argv[1]);  // request server for id of required user
                    /*ostringstream os;
                    os << argv[1] << " id = " << to << endl;
                    out_buf_.push_front(output_line(os.str()));*/
                    if (to) {
                        //out_buf_.push_front(output_line(argv[1]));

                        vector<Message::uid_t> usrlist;

                        usrlist.push_back(parent_->get_model()->get_user_id());
                        usrlist.push_back(to);

                        /*ostringstream oss;
                        oss << usrlist[0] << endl;
                        oss << usrlist[1] << endl;
                        out_buf_.push_front(output_line(oss.str()));*/

                        Message::uid_t room_id = do_newroom(usrlist);
                        /*out_buf_.push_front(output_line("after do_newroom"));

                        ostringstream os;
                        os << "room id = " << room_id << endl;
                        out_buf_.push_front(output_line(os.str()));*/
                        if (room_id) {
                            CursesChat(parent_->get_controller(), parent_->get_model(), room_id,
                                       height_, width_, 0, 0)
                                    .start();
                        }
                    }
                }
            }
            else if (argv[0] == "login") {
                if (argv.size() != 3) {
                    out_buf_.push_front(output_line("Error: wrong command! Format: login <nickname> <passwd>"));
                }
                else if (parent_->get_model()->has_user_id()) {
                    out_buf_.push_front(output_line("Error: you are already logged in!"));
                }
                else { // send login message to server
                    do_login(argv[1], argv[2]);
                }
            }
            else if (argv[0] == "logout") {
                if (parent_->get_model()->get_user_id()) {
                    do_logout();
                    out_buf_.push_front(output_line("Logged out"));
                }
                else
                    out_buf_.push_front(output_line("Error: you are not logged in"));
            }
            else if (cmd == "quit" || cmd == "exit") {
                return 1;
            }
            else {   // error
                string msg = "Error! Type 'help' to see available options";
                out_buf_.push_front(output_line(msg));
            }
            draw_output_panel();
            return 0;
        }

        void CursesTerminal::do_login(const string& nick_name, const string& passwd)
        {
            ClientModel* model = parent_->get_model();
            auto my_id = model->get_user_id();
            string body = nick_name + " " + passwd;
            Message login_msg(Message::MsgType::LOGIN, body, my_id, 0, time(nullptr));
            parent_->get_controller()->send(login_msg);

            out_buf_.push_front(output_line("Login query for nickname \'" + nick_name + "\'..."));
            refresh();

            // wait for server's responce, but no more than 10 sec
            std::condition_variable cv;
            std::mutex cv_m;
            std::unique_lock<std::mutex> lk(cv_m);

            int i = 0;
            auto max_delay = 1s; // 10s
            auto time_step = 50ms;
            auto pred = [this, &cv] () {
                return parent_->get_model()->has_user_id()   // true if user_id received or an error occured
                       || parent_->get_model()->has_error();
            };
            while (!pred() && i*time_step <= max_delay) {
                cv.wait_for(lk, time_step);
                i++;
            }
            if (i*time_step > max_delay) {
                out_buf_.push_front(output_line("Error: login request timeout"));
            }
            else if (model->has_error()) {
                out_buf_.push_front(output_line("Error: " + model->get_error_message()));
                model->reset_error();
            }
            else {
                ostringstream msg;
                msg << "Login  : " + nick_name << "\n";
                msg << "Passwd : " << passwd.c_str() << "\n";
                msg << "User id: " << model->get_user_id();
                model->add_uid_nick_pair(model->get_user_id(), nick_name);
                out_buf_.push_front(output_line(msg.str()));
            }
        }

        void CursesTerminal::do_logout()
        {
            ClientModel* model = parent_->get_model();
            auto my_id = model->get_user_id();
            if (my_id == 0)
                return; // КОСТЫЛИЩЕ!
            Message logout_msg(Message::MsgType::LOGOUT, "", my_id, 0);
            auto netctl = parent_->get_controller();
            netctl->send(logout_msg);
            model->set_user_id(0);
        }

        Message::uid_t CursesTerminal::request_uid(const string& nick)
        {
            ClientModel* model = parent_->get_model();
            NetController* net = parent_->get_controller();
            auto my_id = model->get_user_id();
            Message req_msg(Message::MsgType::UID_REQUEST, nick, my_id, 0);
            net->send(req_msg);

            // wait for response
            std::condition_variable cv;
            std::mutex cv_m;
            std::unique_lock<std::mutex> lk(cv_m);

            int i = 0;
            auto max_delay = 1s; // 10s
            auto time_step = 50ms;
            auto pred = [this, &cv, net] () {
                return net->has_response()   // true if user_id received or an error occured
                       || parent_->get_model()->has_error();
            };
            while (!pred() && i*time_step <= max_delay) {
                cv.wait_for(lk, time_step);
                i++;
            }
            if (i*time_step > max_delay) {
                out_buf_.push_front(output_line("Error: login request timeout"));
            }
            else if (model->has_error()) {
                out_buf_.push_front(output_line("Error: " + model->get_error_message()));
                model->reset_error();
            }
            else {
                string resp = net->get_last_response().get_msg_body();
                std::ofstream f("out.txt");
                f << resp << endl;
                net->reset_last_response();
                istringstream iss(resp);
                Message::uid_t id;
                iss >> id;
                model->add_uid_nick_pair(id, nick);
                f << id << endl;
                f.close();
                return id;
            }
            return 0;
        }

        Message::uid_t CursesTerminal::do_newroom(const std::vector<Message::uid_t>& user_ids)
        {
            auto model = parent_->get_model();
            auto netctl = parent_->get_controller();
            auto my_id = model->get_user_id();

            ostringstream usrlist;
            usrlist << user_ids.size() << " ";
            for (auto i = 0; i < user_ids.size(); i++)
                usrlist << user_ids[i] << " ";

            Message request(Message::MsgType::NEWROOM, usrlist.str(), my_id, 0);
            netctl->send(request);

            // wait for response
            std::condition_variable cv;
            std::mutex cv_m;
            std::unique_lock<std::mutex> lk(cv_m);

            int i = 0;
            auto max_delay = 1s; // 10s
            auto time_step = 50ms;
            auto pred = [this, &cv, netctl] () {
                return netctl->has_response()   // true if user_id received or an error occured
                       || parent_->get_model()->has_error();
            };
            while (!pred() && i*time_step <= max_delay) {
                cv.wait_for(lk, time_step);
                i++;
            }
            if (i*time_step > max_delay) {
                out_buf_.push_front(output_line("Error: create_room request timeout"));
            }
            else if (model->has_error()) {
                out_buf_.push_front(output_line("Error: " + model->get_error_message()));
                model->reset_error();
            }
            else {
                string resp = netctl->get_last_response().get_msg_body();
                netctl->reset_last_response();
                istringstream iss(resp);
                Message::uid_t id;
                iss >> id;
                return id;
            }
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
                // if string starts with substring 'Error', it will be painter in red color
                for (auto j = substrs.rbegin(); j != substrs.rend() && y >= 1; j++) {
                    bool err = boost::starts_with(*j, "Error");
                    if (err)
                        wattron(self_, COLOR_PAIR(ncurses::ColorPair::RED_BLACK));
                    mvwprintw(self_, y, TIME_COL_WIDTH + 1, j->c_str());
                    if (err)
                        wattroff(self_, COLOR_PAIR(ncurses::ColorPair::RED_BLACK));
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

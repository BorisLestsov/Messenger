#ifndef _CHATROOM_DATA_HPP_INCLUDED
#define _CHATROOM_DATA_HPP_INCLUDED

#include <cstddef>
#include <string>
#include <vector>

#include "server_headers/AccountData.hpp"

namespace meow {
    namespace server {

        class ChatroomData {
        public:
            typedef size_t roomid_t;

            ChatroomData(const std::string& name);
            ChatroomData(const std::vector<AccountData::uid_t >& uids);
            ~ChatroomData();

            roomid_t get_room_id() const;
            std::string get_room_name() const;

            void add_user(AccountData::uid_t user_id);
            void add_user(AccountData* user_acc);
            void remove_user(AccountData::uid_t user_id);
            void remove_user(AccountData* user_acc);
            bool has_user(AccountData::uid_t user_id) const;
            bool has_user(AccountData* user_acc) const;
            std::vector<AccountData::uid_t>& get_user_list();
        private:
            roomid_t room_id_;
            std::string room_name_;

            std::vector<AccountData::uid_t> users_in_;
        };
    } // namespace server
} // namespace meow

#endif // _CHATROOM_DATA_HPP_INCLUDED

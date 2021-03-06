#ifndef _SERVER_DATABASE_HPP_INCLUDED
#define _SERVER_DATABASE_HPP_INCLUDED

#include <map>
#include <string>
#include <vector>

#include "AccountData.hpp"
#include "ChatroomData.hpp"

namespace meow {
    namespace server {

        class ServerDatabase {
        public:
            ServerDatabase();
            ~ServerDatabase();

            bool has_account(AccountData::uid_t id) const;
            AccountData* get_account(AccountData::uid_t id);
            AccountData* get_account(const std::string& nick);
            void add_account(const AccountData& new_acc);
            ChatroomData* get_room(std::vector<AccountData::uid_t>& uids);
            ChatroomData* get_room(ChatroomData::roomid_t room_id);
            ChatroomData* get_global_room();
            std::map<AccountData::uid_t, AccountData*>& get_accounts_map();

            void load (const std::string& db_file);
            void store(const std::string& db_file);
        private:
            ChatroomData::roomid_t global_room_id_;
            std::map<AccountData::uid_t, AccountData*> acc_data_;
            std::map<ChatroomData::roomid_t, ChatroomData*> room_data_;
        };

    } // namespace server
} // namespace meow

#endif // _SERVER_DATABASE_HPP_INCLUDED

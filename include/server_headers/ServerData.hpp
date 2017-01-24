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

            void load (const std::string& db_file);
            void store(const std::string& db_file);
        private:
            std::map<AccountData::uid_t, AccountData*> acc_data_;
            std::map<ChatroomData::roomid_t, ChatroomData*> room_data_;
        };

    } // namespace server
} // namespace meow

#endif // _SERVER_DATABASE_HPP_INCLUDED

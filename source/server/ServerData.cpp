#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "server_headers/ServerData.hpp"

namespace meow {
    namespace server {

        using std::cout;
        using std::endl;
        using std::ifstream;
        using std::map;
        using std::ofstream;
        using std::string;

        ServerDatabase::ServerDatabase()
        {
            // on start only GLOBAL room is available
            /*std::vector<AccountData::uid_t> empty;
            ChatroomData* global = new ChatroomData(empty);
            global_room_id_ = global->get_room_id();
            room_data_[global_room_id_] = global;*/
        }

        ServerDatabase::~ServerDatabase()
        {

        }

        bool ServerDatabase::has_account(AccountData::uid_t id) const
        {
            return (bool) acc_data_.count(id);
        }

        AccountData* ServerDatabase::get_account(AccountData::uid_t id)
        {
            auto it = acc_data_.find(id);
            return it != acc_data_.end() ? it->second : nullptr;
        }

        AccountData* ServerDatabase::get_account(const std::string& nick)
        {
            for (auto it = acc_data_.begin(); it != acc_data_.end(); it++)
                if (it->second->get_nick_name() == nick)
                    return it->second;
            return nullptr;
        }

        void ServerDatabase::add_account(const AccountData& new_acc)
        {
            acc_data_[new_acc.get_user_id()] = new AccountData(new_acc);
            store("meow-users.db");
        }

        ChatroomData* ServerDatabase::get_room(std::vector<AccountData::uid_t>& uids)
        {
            // firstly, search for a room with this users
            // then if not found, create new room
            std::sort(uids.begin(), uids.end());
            for (auto p = room_data_.begin(); p != room_data_.end(); p++) {
                auto& v = p->second->get_user_list();
                std::sort(v.begin(), v.end());

                std::vector<AccountData::uid_t> intersection;
                std::set_intersection(v.begin(), v.end(),
                                      uids.begin(), uids.end(),
                                      std::back_inserter(intersection));
                if (intersection.empty())
                    return p->second;
            }
            // not found
            ChatroomData* new_room = new ChatroomData(uids);
            room_data_[new_room->get_room_id()] = new_room;
            return new_room;
        }

        ChatroomData* ServerDatabase::get_global_room()
        {
            return room_data_[global_room_id_];
        }

        std::map<AccountData::uid_t, AccountData*>& ServerDatabase::get_accounts_map()
        {
            return acc_data_;
        }

        ChatroomData* ServerDatabase::get_room(ChatroomData::roomid_t room_id)
        {
            return room_data_[room_id];
        }

        void ServerDatabase::load(const string& db_file)
        {
            ifstream dbfile(db_file);
            if (!dbfile)
                return;

            int n_accs;
            AccountData::uid_t id;
            string nick;
            string md5s;

            dbfile >> n_accs;
            for (int i = 0; i < n_accs; i++) {
                dbfile >> id >> nick >> md5s;
                add_account(AccountData(nick, md5s));
            }
            //cout << "users loaded; len = " << acc_data_.size() << endl;
            dbfile.close();
        }

        void ServerDatabase::store(const string& db_file)
        {
            ofstream to(db_file);
            to << acc_data_.size() << endl;
            for (auto it = acc_data_.begin(); it != acc_data_.end(); it++)
                to << it->first << " " << it->second->get_nick_name() << " " << it->second->get_passwd_hash() << endl;
            to.close();
        }

    } // namespace server
} // namespace meow
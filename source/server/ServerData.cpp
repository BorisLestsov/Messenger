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

        void ServerDatabase::add_account(const AccountData& new_acc)
        {
            acc_data_[new_acc.get_user_id()] = new AccountData(new_acc);
            store("meow-users.db");
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
            cout << "users loaded; len = " << acc_data_.size() << endl;
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
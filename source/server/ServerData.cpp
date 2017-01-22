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

        bool ServerDatabase::add_account(const AccountData& new_acc)
        {
            acc_data_[new_acc.get_user_id()] = new AccountData(new_acc);
            store("meow-users.db");
        }

        void ServerDatabase::load(const string& db_file)
        {

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
#include "server_headers/AccountData.hpp"

#include <string>

#include <openssl/md5.h>

namespace meow {
    namespace server {

        using std::string;

        AccountData::AccountData()
        {

        }

        AccountData::AccountData(const string& nick, const string& passwd)
        {

        }

        AccountData::~AccountData()
        {

        }

        AccountData::uid_t AccountData::get_user_id() const
        {
            return user_id_;
        }

        void AccountData::set_user_id(AccountData::uid_t new_id)
        {
            user_id_ = new_id;
        }

        string AccountData::get_nick_name() const
        {
            return nick_name_;
        }

        void AccountData::set_nick_name(const string& new_nick)
        {
            nick_name_ = new_nick;
        }

        void AccountData::set_passwd(const string& passwd)
        {
            MD5_CTX md5handler;
            string s = nick_name_ + passwd;
            MD5((const unsigned char*) s.c_str(), s.length(), passwd_md5_);
        }

        bool AccountData::check_passwd(const std::string& passwd) const
        {
            return true;
        }

    } // namespace server
} // namespace meow

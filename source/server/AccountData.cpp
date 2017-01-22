#include "server_headers/AccountData.hpp"

#include <cstdio>
#include <iostream>
#include <string>

#include <openssl/md5.h>

namespace meow {
    namespace server {

        using std::cout;
        using std::endl;
        using std::string;

        AccountData::AccountData()
        {
        }

        AccountData::AccountData(const string& nick, const string& passwd)
        {
            nick_name_ = nick;
            std::hash<string> hash_fun;
            user_id_ = hash_fun(nick);
            set_passwd(passwd);
        }

        AccountData::AccountData(const AccountData& other)
        {
            user_id_ = other.user_id_;
            nick_name_ = other.nick_name_;
            for (size_t i = 0; i < MD5_DIGEST_LENGTH; i++)
                passwd_md5_[i] = other.passwd_md5_[i];
        }

        AccountData::~AccountData()
        {
        }

        AccountData& AccountData::operator=(const AccountData &other)
        {
            user_id_ = other.user_id_;
            nick_name_ = other.nick_name_;
            for (size_t i = 0; i < MD5_DIGEST_LENGTH; i++)
                passwd_md5_[i] = other.passwd_md5_[i];
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
            string s = nick_name_ + passwd;
            MD5((const unsigned char*) s.c_str(), s.length(), passwd_md5_);

            char buf[16];
            string result;
            for (int i=0;i<16;i++){
                sprintf(buf, "%02x", passwd_md5_[i]);
                result.append( buf );
            }
            cout << result << endl;
        }

        string AccountData::get_passwd_hash() const
        {
            char buf[16];
            string result;
            for (int i = 0; i < 16; i++) {
                sprintf(buf, "%02x", passwd_md5_[i]);
                result.append(buf);
            }
            return result;
        }

        bool AccountData::check_passwd(const std::string& passwd) const
        {
            return true;
        }

    } // namespace server
} // namespace meow

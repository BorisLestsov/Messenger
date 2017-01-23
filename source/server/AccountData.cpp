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

        AccountData::AccountData(const string& nick, const string& passwd_md5)
        {
            nick_name_ = nick;
            std::hash<string> hash_fun;
            user_id_ = hash_fun(nick);
            passwd_md5_ = passwd_md5;
        }

        AccountData::AccountData(const AccountData& other)
        {
            user_id_ = other.user_id_;
            nick_name_ = other.nick_name_;
            passwd_md5_ = other.passwd_md5_;
        }

        AccountData::~AccountData()
        {
        }

        AccountData& AccountData::operator=(const AccountData &other)
        {
            user_id_ = other.user_id_;
            nick_name_ = other.nick_name_;
            passwd_md5_ = other.passwd_md5_;
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

        string AccountData::get_passwd_hash() const
        {
            return passwd_md5_;
        }

        bool AccountData::check_passwd(const std::string& passwd) const
        {
            return passwd_md5_ == str_to_md5(passwd);
        }

        // transform string to its MD5 representation
        std::string AccountData::str_to_md5(const std::string& s)
        {
            unsigned char md5_digest[MD5_DIGEST_LENGTH];
            MD5((const unsigned char*) s.c_str(), s.length(), md5_digest);

            char buf[MD5_DIGEST_LENGTH];
            string result;
            for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
                sprintf(buf, "%02x", md5_digest[i]);
                result.append(buf);
            }
            return result;
        }

    } // namespace server
} // namespace meow

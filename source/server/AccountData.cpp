#include "server_headers/AccountData.hpp"

#include <cstdio>
#include <iostream>
#include <string>

#include <openssl/md5.h>

#include "Session.hpp"

namespace meow {
    namespace server {

        using std::cout;
        using std::endl;
        using std::string;

        /*AccountData::AccountData()
        {
        }*/

        AccountData::AccountData(const string& nick, const string& passwd_md5)
            :   nick_name_(nick), passwd_md5_(passwd_md5), online_(false), self_session_(nullptr)
        {
            std::hash<string> hash_fun;
            user_id_ = hash_fun(nick);
        }

        AccountData::AccountData(const AccountData& other)
        {
            user_id_ = other.user_id_;
            nick_name_ = other.nick_name_;
            passwd_md5_ = other.passwd_md5_;
            online_ = other.online_;
            self_session_ = other.self_session_;
        }

        AccountData::~AccountData()
        {
        }

        AccountData& AccountData::operator=(const AccountData &other)
        {
            user_id_ = other.user_id_;
            nick_name_ = other.nick_name_;
            passwd_md5_ = other.passwd_md5_;
            self_session_ = other.self_session_;
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

        bool AccountData::is_online() const
        {
            return online_;
        }

        void AccountData::set_online(bool online)
        {
            online_ = online;
        }

        void AccountData::set_session(Session* s)
        {
            self_session_ = s;

            for (auto& m : msg_store_)
                s->deliver(m);
            msg_store_.clear();
        }

        Session* AccountData::get_session()
        {
            return self_session_;
        }

        void AccountData::store_msg(const SerializedMessage& m)
        {
            msg_store_.push_back(m);
        }

        std::vector<SerializedMessage>& AccountData::get_stored_msgs()
        {
            return msg_store_;
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

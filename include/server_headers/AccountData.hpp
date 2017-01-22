#ifndef _ACCOUNT_DATA_HPP_INCLUDED
#define _ACCOUNT_DATA_HPP_INCLUDED

#include <cstddef>
#include <string>

#include <openssl/md5.h>

namespace meow {
    namespace server {

        class AccountData {
        public:
            typedef size_t uid_t;

            AccountData();
            AccountData(const std::string& nick, const std::string& passwd);
            ~AccountData();

            uid_t get_user_id() const;
            void set_user_id(uid_t new_id);
            std::string get_nick_name() const;
            void set_nick_name(const std::string& new_nick);
            void set_passwd(const std::string& passwd);
            bool check_passwd(const std::string& passwd) const;
        private:
            uid_t user_id_;
            std::string nick_name_;
            unsigned char passwd_md5_[MD5_DIGEST_LENGTH];
        };
    } // namespace server
} // namespace meow

#endif // _ACCOUNT_DATA_HPP_INCLUDED

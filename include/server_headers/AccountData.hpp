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
            AccountData(const std::string& nick, const std::string& passwd_md5);
            AccountData(const AccountData& other);
            ~AccountData();

            AccountData& operator=(const AccountData& other);

            uid_t get_user_id() const;
            void set_user_id(uid_t new_id);
            std::string get_nick_name() const;
            void set_nick_name(const std::string& new_nick);
            std::string get_passwd_hash() const;
            bool check_passwd(const std::string& passwd) const;

            static std::string str_to_md5(const std::string& s);
        private:
            uid_t user_id_;
            std::string nick_name_;
            std::string passwd_md5_;
        };
    } // namespace server
} // namespace meow

#endif // _ACCOUNT_DATA_HPP_INCLUDED

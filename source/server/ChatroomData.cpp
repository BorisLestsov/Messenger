#include "server_headers/AccountData.hpp"
#include "server_headers/ChatroomData.hpp"

#include <string>

namespace meow {
    namespace server {

        using std::string;

        ChatroomData::ChatroomData(const string& name)
        {
            std::hash<string> hash_fun;
            room_id_ = hash_fun(name);
            room_name_ = name;
        }

        ChatroomData::~ChatroomData()
        {

        }

        ChatroomData::roomid_t ChatroomData::get_room_id() const
        {
            return room_id_;
        }

        string ChatroomData::get_room_name() const
        {
            return room_name_;
        }

        void ChatroomData::add_user(AccountData::uid_t user_id)
        {
            if (!has_user(user_id))
                users_in_.push_back(user_id);
        }

        void ChatroomData::add_user(AccountData* user_acc)
        {
            add_user(user_acc->get_user_id());
        }

        void ChatroomData::remove_user(AccountData::uid_t user_id)
        {
            for (auto p = users_in_.begin(); p != users_in_.end(); p++)
                if (*p == user_id) {
                    users_in_.erase(p);
                    break;
                }
        }

        void ChatroomData::remove_user(AccountData* user_acc)
        {
            remove_user(user_acc->get_user_id());
        }

        bool ChatroomData::has_user(AccountData::uid_t user_id) const
        {
            for (auto id : users_in_)
                if (id == user_id)
                    return true;
            return false;
        }

        bool ChatroomData::has_user(AccountData* user_acc) const
        {
            return has_user(user_acc->get_user_id());
        }

    } // namespace server
} // namespace meow

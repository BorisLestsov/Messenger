#ifndef CHAT_MESSAGE_HPP
#define CHAT_MESSAGE_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>

#include <string>

namespace meow {

    using namespace std;

    class Message;

    class SerializedMessage {
    public:
        static const ulong HEADER_LENGTH;

        SerializedMessage(const Message &);

        ~SerializedMessage();

        char *get_buf() const;
        // const char* operator() () const;
        //operator void *();

        friend class Message;

        ulong get_msg_len() const;
        ulong get_body_len() const;

    private:
        ulong msg_length;
        ulong body_length;
        char *buf;
    };


    class Message {
    public:
        typedef ulong uid_t;
        typedef time_t send_date_t;

        static const ulong MAX_BODY_LENGTH = 512;

        enum MsgType {
            EMPTY = 0, TEXT, LOGIN, LOGOUT, CREATE_ROOM, INVITE
        };
        // For debug
        static const char *msg_type_names[];

        Message();
        Message(MsgType, const string &, uid_t from, uid_t to, send_date_t);
        Message(MsgType, const char *, uid_t from, uid_t to, send_date_t);
        Message(const Message &);

        const Message &operator=(const Message &);

        // TODO: add move constructor and move operator

        explicit Message(const SerializedMessage &);
        SerializedMessage serialize() const;

        friend ostream &operator<<(ostream &o, const Message &);
        friend class SerializedMessage;

        MsgType get_msg_type() const;
        const string &get_msg_body() const;

    private:
        MsgType msg_type_;
        uid_t uid_from_;
        uid_t uid_to_;
        send_date_t sending_date_;
        string msg_body_;
    };



    ostream &operator<<(ostream &o, const Message &msg);

    typedef std::deque<Message> chat_message_queue;
}

#endif // CHAT_MESSAGE_HPP

#ifndef CHAT_MESSAGE_HPP
#define CHAT_MESSAGE_HPP

#include <stdexcept>

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
        static const size_t HEADER_LENGTH;
        static const size_t MAX_MSG_LENGTH;

        SerializedMessage();
        SerializedMessage(const Message &);
        ~SerializedMessage();

        void decode_msg_length();

        char *get_buf();
        char *get_body_buf();
        // const char* operator() () const;
        //operator void *();

        size_t get_msg_len() const;
        size_t get_body_len() const;

        void resize(size_t length);

        friend class Message;

    private:
        static const ptrdiff_t LENGTH_OFFSET_;

        size_t msg_length_;
        size_t body_length_;
        char *buf;
    };


    class Message {
    public:
        typedef size_t uid_t;
        typedef time_t send_date_t;

        static const size_t MAX_BODY_LENGTH = 512;

        enum MsgType {
            EMPTY = 0, TEXT, LOGIN, LOGOUT, NEWROOM, INVITE, UID_REQUEST, ERROR
        };
        // For debug
        static const char *msg_type_names[];

        Message();
        Message(MsgType, const string &, uid_t from, uid_t to);
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
        uid_t get_to_uid() const;
        uid_t get_from_uid() const;

        string get_date(string format) const;

    private:
        MsgType msg_type_;
        uid_t uid_from_;
        uid_t uid_to_;
        send_date_t sending_date_;
        string msg_body_;
    };



    ostream &operator<<(ostream &o, const Message &msg);

    typedef std::deque<SerializedMessage> chat_message_queue; // TODO: WTF?
}

#endif // CHAT_MESSAGE_HPP

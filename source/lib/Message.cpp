#include "Message.hpp"
#include <iostream>
#include <iomanip>

namespace meow {

    const ulong SerializedMessage::HEADER_LENGTH = sizeof(Message::msg_type_) +
                                                   sizeof(Message::uid_from_) +
                                                   sizeof(Message::uid_to_) +
                                                   sizeof(Message::sending_date_) +
                                                   sizeof(SerializedMessage::body_length);


    SerializedMessage::SerializedMessage(const Message &msg) {
        buf = new char[sizeof(msg.msg_type_) + msg.msg_body_.size()];
        char *ptr = buf;

        memcpy(ptr, &msg.msg_type_, sizeof(msg.msg_type_));
        ptr += sizeof(msg.msg_type_);
        memcpy(ptr, &msg.uid_from_, sizeof(msg.uid_from_));
        ptr += sizeof(msg.uid_from_);
        memcpy(ptr, &msg.uid_to_, sizeof(msg.uid_to_));
        ptr += sizeof(msg.uid_to_);
        memcpy(ptr, &msg.sending_date_, sizeof(msg.sending_date_));
        ptr += sizeof(msg.sending_date_);

        body_length = msg.msg_body_.size();
        memcpy(ptr, &body_length, sizeof(body_length));
        ptr += sizeof(body_length);
        memcpy(ptr, msg.msg_body_.data(), body_length);
        ptr += msg.msg_body_.size();

        msg_length = ptr - buf;
    }

    SerializedMessage::~SerializedMessage() {
        delete []buf;
    }

    char *SerializedMessage::get_buf() const {
        return buf;
    }

    /*SerializedMessage::operator void *() {
        return buf;
    }*/

    ulong SerializedMessage::get_msg_len() const {
        return msg_length;
    }

    ulong SerializedMessage::get_body_len() const {
        return body_length;
    }


    Message::Message() :
            msg_type_(MsgType::EMPTY),
            msg_body_() {}

    Message::Message(MsgType t, const string &str, uid_t from, uid_t to, send_date_t date) :
            msg_type_(t),
            uid_from_(from),
            uid_to_(to),
            sending_date_(date),
            msg_body_(str) {}

    Message::Message(MsgType t, const char *str, uid_t from, uid_t to, send_date_t date) :
            msg_type_(t),
            uid_from_(from),
            uid_to_(to),
            sending_date_(date),
            msg_body_(str) {}

    Message::Message(const Message &msg) :
            msg_type_(msg.msg_type_),
            uid_from_(msg.uid_from_),
            uid_to_(msg.uid_to_),
            sending_date_(msg.sending_date_),
            msg_body_(msg.msg_body_) {}


    const Message &Message::operator=(const Message &msg) {
        msg_type_ = msg.msg_type_;
        uid_from_ = msg.uid_from_;
        uid_to_ = msg.uid_to_;
        sending_date_ = msg.sending_date_;
        msg_body_ = msg.msg_body_;

        return *this;
    }


    Message::Message(const SerializedMessage &ser_msg){
        char* buf = ser_msg.buf;
        char *ptr = buf;

        memcpy(&msg_type_, ptr, sizeof(msg_type_));
        ptr += sizeof(msg_type_);
        memcpy(&uid_from_, ptr, sizeof(uid_from_));
        ptr += sizeof(uid_from_);
        memcpy(&uid_to_, ptr, sizeof(uid_to_));
        ptr += sizeof(uid_to_);
        memcpy(&sending_date_, ptr, sizeof(sending_date_));
        ptr += sizeof(sending_date_);

        ulong body_len;
        memcpy(&body_len, ptr, sizeof(body_len));
        ptr += sizeof(body_len);
        msg_body_ = string(ptr, ser_msg.body_length);
    }

    SerializedMessage Message::serialize() const {
        return SerializedMessage(*this);
    }


    ostream& operator<<(ostream &o, const Message &msg) {
        o << "Type: " << left << setw(15) << Message::msg_type_names[msg.msg_type_]
          << "From: " << left << setw(8) << msg.uid_from_
          << "To: " << left << setw(8) << msg.uid_to_
          << "Date: " << left << setw(8) << msg.sending_date_ << endl;
        o << msg.msg_body_ << endl;
        return o;
    }

    Message::MsgType Message::get_msg_type() const {
        return msg_type_;
    }

    const string &Message::get_msg_body() const {
        return msg_body_;
    }


    const char *Message::msg_type_names[] = {"EMPTY", "TEXT", "LOGIN", "LOGOUT", "CREATE_ROOM", "INVITE"};
}



/*
const char* Message::data() const {
    return data_;
}

char *data() {
    return data_;
}

std::size_t length() const {
    return header_length + body_length_;
}

const char *body() const {
    return data_ + header_length;
}

char *body() {
    return data_ + header_length;
}

std::size_t body_length() const {
    return body_length_;
}

void body_length(std::size_t new_length) {
    body_length_ = new_length;
    if (body_length_ > max_body_length)
        body_length_ = max_body_length;
}

bool decode_header() {
    char header[header_length + 1] = "";
    std::strncat(header, data_, header_length);
    body_length_ = std::atoi(header);
    if (body_length_ > max_body_length) {
        body_length_ = 0;
        return false;
    }
    return true;
}

void encode_header() {
    char header[header_length + 1] = "";
    std::sprintf(header, "%4lu", body_length_);
    std::memcpy(data_, header, header_length);
}

*/
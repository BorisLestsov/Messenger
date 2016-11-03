//
// Created by boris on 03.11.16.
//

#include "Message.h"

using namespace Msg;

Message::Message(): msg_() {

}

Message::Message(const std::string& str): msg_(str) {

}

ostream &Msg::operator<<(ostream &o, const Message & msg) {
    o << msg.msg_;
    return o;
}


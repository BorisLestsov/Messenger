#include "lib_headers/Message.hpp"
#include <iostream>
#include <ctime>

using namespace meow;

int main(int argc, char**argv) {
    Message s(Message::MsgType::TEXT, "Dratuti)))00", 5,6,7);


//    SerializedMessage ser;
//    ser.resize(512);
//
//    SerializedMessage mm = s.serialize();
//
//    memcpy(ser.get_buf(), mm.get_buf(), SerializedMessage::HEADER_LENGTH);
//    ser.decode_msg_length();
//    cout << ser.get_body_len() << endl;

    return 0;
}


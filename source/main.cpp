#include <Message.hpp>
#include <iostream>
#include <ctime>

using namespace meow;

int main(int argc, char**argv) {
    Message s(Message::MsgType::TEXT, "Dratuti)))00", 5,6,7);

    SerializedMessage ser = s.serialize();

    Message msg(ser);

    cout << msg;

    return 0;
}


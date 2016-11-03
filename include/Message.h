//
// Created by boris on 03.11.16.
//

#ifndef MESSENGER_MESSAGE_H
#define MESSENGER_MESSAGE_H

#include <string>
#include <iostream>

namespace Msg {

    using namespace std;

    class Header {
        // Something should be here
    };

    class Message {
        Header head_;
        string msg_;

    public:
        Message();
        Message(const string&);

        friend ostream& operator<<(ostream&, const Message&);
    };

    ostream& operator<<(ostream&, const Message&);
}

#endif //MESSENGER_MESSAGE_H

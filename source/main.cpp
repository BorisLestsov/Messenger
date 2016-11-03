#include <iostream>
#include <boost/asio.hpp>
#include "Message.h"

using Msg::Message;
using std::cout;
using std::endl;

int main() {
    Message msg("main message!");

    // Comment in the new "dev" branch

    cout << msg << endl;

    return 0;
}

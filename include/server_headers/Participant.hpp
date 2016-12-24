#ifndef _CHAT_PARTICIPANT_HPP_INCLUDED
#define _CHAT_PARTICIPANT_HPP_INCLUDED

#include <memory>

#include "lib_headers/Message.hpp"

namespace meow {
    namespace server {

        class Participant {
        public:
            typedef uint32_t uid;

            virtual ~Participant();

            virtual void deliver(const SerializedMessage &msg) = 0;
        };

        typedef std::shared_ptr <Participant> chat_participant_ptr;

    }
} // namespace meow

#endif // _CHAT_PARTICIPANT_HPP_INCLUDED

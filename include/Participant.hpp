#ifndef _CHAT_PARTICIPANT_HPP_INCLUDED
#define _CHAT_PARTICIPANT_HPP_INCLUDED

#include <memory>

#include "Message.hpp"

namespace meow {

class Participant {
public:
  virtual ~Participant();
  virtual void deliver(const Message& msg) = 0;
};

typedef std::shared_ptr<Participant> chat_participant_ptr;

} // namespace meow

#endif // _CHAT_PARTICIPANT_HPP_INCLUDED

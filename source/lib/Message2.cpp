#include <cstring>
#include <ctime>
#include <stdexcept>
#include <string>

#include "Message2.hpp"

namespace meow {
	
	const Message2::uid_t Message2::SERVER_UID = 0;
	
	const Message2::version_t Message2::VERSION_1_0  = 1;
	const Message2::msgtype_t Message2::TYPE_TEXT    = 0;
	const Message2::msgtype_t Message2::TYPE_LOGIN   = 1;
	const Message2::msgtype_t Message2::TYPE_LOGOUT  = 2;
	const Message2::msgtype_t Message2::TYPE_NEWROOM = 3;
	const Message2::msgtype_t Message2::TYPE_INVITE  = 4;
	
	Message2::Message2()
	:	type_(TYPE_TEXT),
		proto_version_(VERSION_1_0),
		uid_from_(SERVER_UID),
		uid_to_(SERVER_UID),
		sending_date_(time(nullptr)),
		len_(0),
		body_(nullptr)
	{
	}
	
	Message2::Message2(char* raw_bytes)
		:	Message2()
	{
		size_t required_min_len = header_size();
		size_t actual_len = strlen(raw_bytes);
		if (actual_len < required_min_len)
			throw std::invalid_argument("while deserialization");
	}
	
	size_t Message2::size() const
	{
		return header_size() + strlen(body_);
	}
	
	size_t Message2::header_size() const
	{
		return
			sizeof(Message2::msgtype_t) +
			sizeof(Message2::version_t) +
			2*sizeof(Message2::uid_t) +
			sizeof(Message2::send_date_t) +
			sizeof(size_t);
	}
	
	size_t Message2::serialize(char* bytes) const
	{
		size_t len = size();
		// TODO: serizalization code
		return len;
	}
    
} // namespace meow

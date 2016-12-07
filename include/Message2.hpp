#ifndef _MESSAGE_HPP_INCLUDED
#define _MESSAGE_HPP_INCLUDED

#include <ctime>
#include <string>

namespace meow {

    class Message2 {
    public:
		typedef uint32_t      uid_t;
		typedef time_t        send_date_t;
		typedef unsigned char msgtype_t;
		typedef unsigned char version_t;
		
		static const uid_t SERVER_UID;
		
		static const version_t VERSION_1_0;
		static const msgtype_t TYPE_TEXT;
		static const msgtype_t TYPE_LOGIN;
		static const msgtype_t TYPE_LOGOUT;
		static const msgtype_t TYPE_NEWROOM;
		static const msgtype_t TYPE_INVITE;
		
		Message2();
		size_t size() const;
		size_t serialize(char* bytes) const;
	private:
		// header
		msgtype_t type_;
		version_t proto_version_;
		uid_t uid_from_;
		uid_t uid_to_;
		send_date_t sending_date_;
		size_t len_;
		// body
		char* body_;
    };
    
} // namespace meow

#endif // _MESSAGE_HPP_INCLUDED


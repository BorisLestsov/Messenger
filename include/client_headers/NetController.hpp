#ifndef _NET_CONTROLLER_HPP_INCLUDED
#define _NET_CONTROLLER_HPP_INCLUDED

#include <deque>
#include <thread>

#include <boost/asio.hpp>

#include "ClientModel.hpp"
#include "lib_headers/Message.hpp"

namespace meow {
	namespace client {

		class NetController {
		public:
			NetController(ClientModel* model);

			~NetController();

			void send(const Message &msg);
			void open_connection(char* address, char* port);
			void write(const Message& msg);
			void close_connection();
			void do_read_header();
			void do_read_body();
			void do_write();

		private:
			ClientModel* model_;
            
            std::thread* self_thread_;
            boost::asio::ip::tcp::socket* socket_;
			
			boost::asio::io_service io_service_;
            SerializedMessage msg_buf_;
            std::deque<SerializedMessage> write_msgs_;
		}; // class NetController

	}
} // namespace meow


#endif // _NET_CONTROLLER_HPP_INCLUDED

#include <deque>
#include <exception>
#include <iostream>
#include <thread>

#include <boost/asio.hpp>

#include "client_headers/NetController.hpp"
#include "lib_headers/Message.hpp"

namespace meow {
	namespace client {
		
		using std::cout;
		using std::cin;
		using std::cerr;
		using std::endl;
		using std::exception;
		using boost::asio::ip::tcp;
		
		NetController::NetController(ClientModel* model)
			:   model_(model),
                socket_(nullptr),
                self_thread_(nullptr),
                 io_service_(),
                msg_buf_()
                // field write_msgs_ are constructed implicitly
		{
			msg_buf_.resize(SerializedMessage::MAX_MSG_LENGTH);
		}
		
		void NetController::open_connection(char* address, char* port)
		{
			boost::asio::ip::tcp::resolver resolver(io_service_);
			auto endpoint_iterator = resolver.resolve({address, port});
			
			if (socket_) {
				socket_->close();
				delete socket_;
			}
			socket_ = new boost::asio::ip::tcp::socket(io_service_);

			auto connect_handler = [this](boost::system::error_code ec, tcp::resolver::iterator) {
                if (!ec) {
                    do_read_header();
                }
                else {
                    cout << "connection error" << endl;
                }
            };

            boost::asio::async_connect(*socket_,
                                       endpoint_iterator,
                                       connect_handler);

            self_thread_ = new std::thread([this]() {
                io_service_.run();
            });
		}

        void NetController::send(const Message& msg)
        {
            write(msg);
        }

        void NetController::write(const Message& msg)
        {
            auto write_f = [this, msg]() {
                bool write_in_progress = !write_msgs_.empty();

                write_msgs_.push_back(msg.serialize());

                if (!write_in_progress)
                    do_write();
            };

            io_service_.post(write_f);
        }

        void NetController::close_connection()
        {
            io_service_.post([this]() { socket_->close(); });
        }

        void NetController::do_read_header()
        {
            auto read_header_f = [this](boost::system::error_code ec,
                                        std::size_t length) {
                if (!ec) {
                    msg_buf_.decode_msg_length();
                    do_read_body();
                } else {
                    socket_->close();
                }
            };

            boost::asio::async_read(*socket_,
                                    boost::asio::buffer(msg_buf_.get_buf(),
                                                        SerializedMessage::HEADER_LENGTH),
                                    read_header_f
            );
        }

        void NetController::do_read_body()
        {
            auto read_body_f = [this](boost::system::error_code ec,
                                      std::size_t length) {
                if (!ec) {
//                std::cout.write(msg_buf.get_body_buf(), msg_buf.get_body_len());
//                std::cout << endl;
                    Message msg(msg_buf_);
                    cout << msg;

                    do_read_header();
                } else
                    socket_->close();
            };


            boost::asio::async_read(*socket_,
                                    boost::asio::buffer(msg_buf_.get_body_buf(),
                                                        msg_buf_.get_body_len()),
                                    read_body_f
            );
        }

        void NetController::do_write() {
            auto write_f = [this](boost::system::error_code ec,
                                  std::size_t length) {
                if (!ec) {
                    write_msgs_.pop_front();
                    if (!write_msgs_.empty()) {
                        do_write();
                    }
                } else
                    socket_->close();
            };

            SerializedMessage msg_buf = write_msgs_.front();

            boost::asio::async_write(*socket_,
                                     boost::asio::buffer(msg_buf.get_buf(),
                                                         msg_buf.get_msg_len()),
                                     write_f
            );
        }

		NetController::~NetController()
		{
		}

	}
} // namespace meow

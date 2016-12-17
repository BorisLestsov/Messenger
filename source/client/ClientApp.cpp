#include <iostream>

#include "client_headers/ClientApp.hpp"
#include "client_headers/ClientModel.hpp"
#include "client_headers/ClientView.hpp"
#include "client_headers/NcursesView.hpp"
#include "client_headers/NetController.hpp"
#include "lib_headers/Message.hpp"

namespace meow {
	namespace client {
		
		using std::cout;
		using std::cin;
		using std::endl;

		const int ClientApp::CONSOLE_APP = 0;
		const int ClientApp::NCURSES_APP = 1;
		const int ClientApp::QT_APP = 2;

		ClientApp::ClientApp()
		{
			model_ = new ClientModel();
			controller_ = new NetController(model_);
			view_ = new NcursesView(controller_, model_);
            model_->add_observer(view_);
		}
		
		int ClientApp::main(int argc, char** argv)
		{
			controller_->open_connection(argv[0], argv[1]);
			
			/*char line[Message::MAX_BODY_LENGTH + 1];
			while (std::cin.getline(line, Message::MAX_BODY_LENGTH + 1)) {
				Message msg(Message::MsgType::TEXT, line, 42, 69, 100500);

				//msg.encode_header();
				controller_->write(msg);
			}*/
			view_->start();

			controller_->close_connection();
			//t.join();
			
			return 0;
		}

		ClientApp::~ClientApp()
		{
			delete model_;
			delete view_;
			delete controller_;
		}

	}
} // namespace meow




#include "client_headers/ClientApp.hpp"
#include "client_headers/ClientModel.hpp"
#include "client_headers/ClientView.hpp"
#include "client_headers/NcursesView.hpp"
#include "client_headers/NetController.hpp"

namespace meow {
	namespace client {

		const int ClientApp::CONSOLE_APP = 0;
		const int ClientApp::NCURSES_APP = 1;
		const int ClientApp::QT_APP = 2;

		ClientApp::ClientApp()
		{
			model_ = new ClientModel();
			controller_ = new NetController(model_);
			view_ = new NcursesView(controller_, model_);
		}
		
		int ClientApp::main(int argc, char** argv)
		{
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




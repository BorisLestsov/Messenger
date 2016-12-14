#include "ClientApp.hpp"
#include "ClientModel.hpp"
#include "ClientView.hpp"
#include "NetController.hpp"

namespace meow {
	namespace client {

		const int ClientApp::CONSOLE_APP = 0;
		const int ClientApp::NCURSES_APP = 1;
		const int ClientApp::QT_APP = 2;

		ClientApp::ClientApp() {
			// dummy code
			model = nullptr;
			view = nullptr;
			controller = nullptr;
		}

		ClientApp::~ClientApp() {
			delete model;
			delete view;
			delete controller;
		}

	}
} // namespace meow




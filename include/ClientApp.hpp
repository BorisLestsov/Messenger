#pragma once
#ifndef _CLIENT_APP_HPP_INCLUDED
#define _CLIENT_APP_HPP_INCLUDED

#include "ClientModel.hpp"
#include "ClientView.hpp"
#include "NetController.hpp"

namespace meow {

	class ClientApp {
	public:
		static const int CONSOLE_APP;
		static const int NCURSES_APP;
		static const int QT_APP;
		
		ClientApp();
		~ClientApp();
	private:
		NetController* controller;
		ClientModel* model;
		ClientView* view;
	}; // class ClientApp

} // namespace meow


#endif // _CLIENT_APP_HPP_INCLUDED


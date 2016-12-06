#pragma once
#ifndef _CLIENT_VIEW_HPP_INCLUDED
#define _CLIENT_VIEW_HPP_INCLUDED

#include "NetController.hpp"
#include "Observer.hpp"

namespace meow {

	class ClientView : public Observer {
	protected:
		ClientView(NetController*);
	private:
		NetController* controller;
	}; // abstract class ClientView

} // namespace meow


#endif // _CLIENT_VIEW_HPP_INCLUDED

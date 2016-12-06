#pragma once
#ifndef _NET_CONTROLLER_HPP_INCLUDED
#define _NET_CONTROLLER_HPP_INCLUDED

#include "ClientModel.hpp"
#include "Message.hpp"

namespace meow {

	class NetController {
	public:
		NetController();
		~NetController();
		void send(Message& msg);
	private:
		ClientModel* model;
	}; // class NetController

} // namespace meow


#endif // _NET_CONTROLLER_HPP_INCLUDED

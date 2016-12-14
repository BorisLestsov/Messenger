#ifndef _CLIENT_VIEW_HPP_INCLUDED
#define _CLIENT_VIEW_HPP_INCLUDED

#include "ClientModel.hpp"
#include "NetController.hpp"
#include "Observer.hpp"

namespace meow {

	class ClientView : public Observer {
	public:
		virtual ~ClientView();
	protected:
		ClientView(NetController*, ClientModel*);
	private:
		NetController* controller_;
		ClientModel* model_;
	}; // abstract class ClientView

} // namespace meow


#endif // _CLIENT_VIEW_HPP_INCLUDED

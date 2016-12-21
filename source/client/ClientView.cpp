#include "client_headers/ClientModel.hpp"
#include "client_headers/ClientView.hpp"
#include "client_headers/NetController.hpp"
#include "lib_headers/Message.hpp"

namespace meow {
	namespace client {

		ClientView::ClientView(NetController *controller, ClientModel *model)
				: controller_(controller), model_(model)
		{
            model_->add_observer(this);
		}

		void ClientView::send(const Message& msg)
		{
			controller_->send(msg);
		}

		ClientView::~ClientView()
		{
		}

	}
} // namespace meow

#include "client_headers/ClientModel.hpp"
#include "client_headers/ClientUI.hpp"
#include "client_headers/NetController.hpp"
#include "lib_headers/Message.hpp"

namespace meow {
	namespace client {

		ClientUI::ClientUI(NetController *controller, ClientModel *model)
				: controller_(controller), model_(model)
		{
            model_->add_observer(this);
		}

		void ClientUI::send(const Message& msg)
		{
			controller_->send(msg);
		}

		ClientUI::~ClientUI()
		{
		}

	}
} // namespace meow

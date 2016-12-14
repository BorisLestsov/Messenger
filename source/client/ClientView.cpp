#include "client_headers/ClientModel.hpp"
#include "client_headers/ClientView.hpp"
#include "client_headers/NetController.hpp"

namespace meow {
	namespace client {

		ClientView::ClientView(NetController *controller, ClientModel *model)
				: controller_(controller), model_(model) {
		}

		ClientView::~ClientView() {
		}

	}
} // namespace meow

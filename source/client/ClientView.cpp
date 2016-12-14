#include "ClientModel.hpp"
#include "ClientView.hpp"
#include "NetController.hpp"

namespace meow {
	namespace client {

		ClientView::ClientView(NetController *controller, ClientModel *model)
				: controller_(controller), model_(model) {
		}

		ClientView::~ClientView() {
		}

	}
} // namespace meow

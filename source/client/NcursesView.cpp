#include "client_headers/ClientModel.hpp"
#include "client_headers/ClientView.hpp"
#include "client_headers/NcursesView.hpp"
#include "client_headers/NetController.hpp"

namespace meow {
	namespace client {
		
		NcursesView::NcursesView(NetController* netctl, ClientModel* model)
			:	ClientView(netctl, model)
		{
		}
			
		void NcursesView::update()
		{
		}
			
		NcursesView::~NcursesView()
		{
		}

	} // namespace client
} // namespace meow

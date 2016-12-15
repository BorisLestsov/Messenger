#ifndef _NCURSES_VIEW_HPP_INCLUDED
#define _NCURSES_VIEW_HPP_INCLUDED

#include "ClientModel.hpp"
#include "ClientView.hpp"
#include "NetController.hpp"

namespace meow {
	namespace client {

		class NcursesView : public ClientView {
		public:
			NcursesView(NetController*, ClientModel*);
			
			void update();
			
			~NcursesView();

		}; // class NcursesView

	} // namespace client
} // namespace meow


#endif // _CLIENT_VIEW_HPP_INCLUDED


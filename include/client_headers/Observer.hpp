#ifndef _OBSERVER_HPP_INCLUDED
#define _OBSERVER_HPP_INCLUDED

namespace meow {
	namespace client {

		class Observer {
		public:
			virtual void update() = 0;
		};

	}
} // namespace meow


#endif // _OBSERVER_HPP_INCLUDED

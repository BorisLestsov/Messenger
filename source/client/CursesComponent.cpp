#include "client_headers/CursesComponent.hpp"
#include "lib_headers/ncurses-api.hpp"

namespace meow {
    namespace client {

        // no keyboard capture by default
        CursesComponent::CursesComponent(int height, int width, int starty, int startx)
            :   width_(width),
                height_(height)
        {
            self_ = ncurses::newwin(height, width, starty, startx);
        }

        CursesComponent::~CursesComponent()
        {
            delwin(self_);
        }

    } // namespace meow::client
} // namespace meow


//
// @author magicxqq <xqq@xqq.im>
//

#ifndef DRAWBOARD_CLIENT_APP_HPP
#define DRAWBOARD_CLIENT_APP_HPP

#include "sdl_include.hpp"

class ClientApp {
public:
    int Run();
private:
    SDL_Window* window = nullptr;
    SDL_Surface* screen = nullptr;
};


#endif // DRAWBOARD_CLIENT_APP_HPP

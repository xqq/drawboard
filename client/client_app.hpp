//
// @author magicxqq <xqq@xqq.im>
//

#ifndef DRAWBOARD_CLIENT_APP_HPP
#define DRAWBOARD_CLIENT_APP_HPP

#include <common/abstract_canvas.hpp>
#include "sdl_include.hpp"

class ClientApp {
public:
    int Run();
private:
    SDL_Window* window = nullptr;
    SDL_Surface* screen = nullptr;
    SDL_Texture* texture = nullptr;
    AbstractCanvas canvas_;
};


#endif // DRAWBOARD_CLIENT_APP_HPP

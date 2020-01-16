//
// @author magicxqq <xqq@xqq.im>
//

#include <iostream>
#include <sstream>
#include "client_app.hpp"

constexpr int viewport_width = 1280;
constexpr int viewport_height = 720;

ClientApp::ClientApp(std::string host, uint16_t port) : host_(host), port_(port) { }

int ClientApp::Run() {
    if (SDL_Init(SDL_INIT_VIDEO) == -1) {
        std::cout << SDL_GetError() << std::endl;
        return 1;
    }

    std::ostringstream title_stream;
    title_stream << "Drawboard - Server: " << host_ << ":" << port_;
    std::string title = title_stream.str();

    SDL_Window* window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, viewport_width, viewport_height, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cout << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Renderer *renderer = nullptr;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr){
        std::cout << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, viewport_width, viewport_height);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    client_.Login(host_, port_);
    client_.InitViewport(viewport_width, viewport_height);

    bool left_mouse_down = false;

    while (true) {
        SDL_Event event;
        if (SDL_PollEvent(&event) == 1) {
            if (event.type == SDL_QUIT) {
                break;
            }

            switch (event.type) {
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_z) {
                        const uint8_t* keystates = SDL_GetKeyboardState(nullptr);
                        if (keystates[SDL_SCANCODE_LCTRL]) {
                            // Ctrl + Z
                            client_.UndoLast();
                        }
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        left_mouse_down = false;
                        client_.EndDraw();
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        if (!left_mouse_down) {
                            client_.BeginDraw();
                            left_mouse_down = true;
                        }
                    }
                case SDL_MOUSEMOTION:
                    if (left_mouse_down) {
                        int mouseX = event.motion.x;
                        int mouseY = event.motion.y;
                        client_.DrawPoint(Point(mouseX, mouseY));
                    }
                    break;
            }
        }

        client_.Render(viewport_width, viewport_height);

        SDL_UpdateTexture(texture, NULL, client_.GetPixelBuffer(), viewport_width * sizeof(uint32_t));

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    client_.Logout();

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
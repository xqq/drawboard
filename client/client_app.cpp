//
// @author magicxqq <xqq@xqq.im>
//

#include <iostream>
#include "client_app.hpp"

constexpr int width = 1280;
constexpr int height = 720;

int ClientApp::Run() {
    if (SDL_Init(SDL_INIT_VIDEO) == -1) {
        std::cout << SDL_GetError() << std::endl;
        return 1;
    }

    window = SDL_CreateWindow("Drawboard", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
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

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 1280, 720);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    bool left_mouse_down = false;
    uint32_t sequence_id = 0;

    canvas_.InitPixelBuffer(1280, 720);

    while (true) {
        SDL_Event event;
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) {
            break;
        } else {
            switch (event.type) {
                case SDL_MOUSEBUTTONUP:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        left_mouse_down = false;
                        canvas_.EndDraw(0, sequence_id);
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        sequence_id++;
                        canvas_.BeginDraw(0, sequence_id, 0);
                        left_mouse_down = true;
                    }
                case SDL_MOUSEMOTION:
                    if (left_mouse_down) {
                        int mouseX = event.motion.x;
                        int mouseY = event.motion.y;
                        canvas_.DrawPoint(0, sequence_id, Point(mouseX, mouseY));
                    }
                    break;
            }
        }

        canvas_.Render(1280, 720);

        SDL_UpdateTexture(texture, NULL, canvas_.GetPixelBuffer(), 1280 * sizeof(uint32_t));

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
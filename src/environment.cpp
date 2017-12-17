//
// Created by Avikam Agur on 10/11/2017.
//

#include <SDL2/SDL_opengl.h>
#include <iostream>
#include "src/constants.h"
#include "src/environment.h"

namespace pong{
    input_t environment::get_event() {
        auto r = pm_ct.poll_event();

        // Handle events.
        SDL_Event event {};
        while (SDL_PollEvent(&event)) {
            // Clicking 'x' or pressing F4.
            if (event.type == SDL_QUIT) {
                return input_t::quit;
            }

            // Pressing a key.
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    // Pressing ESC exits from the game.
                    case SDLK_ESCAPE:
                        r |=input_t::quit;
                        break;
                    case SDLK_SPACE:
                        r |= (input_t::p1_press | input_t::p2_press);
                        break;
                    case SDLK_UP:
                        r |=input_t::player_1_up;
                        break;
                    case SDLK_DOWN:
                        r |=input_t::player_1_down;
                        break;
                    case SDLK_q:
                        r |= input_t::player_2_down;
                        break;
                    case SDLK_a:
                        r |= input_t::player_2_up;
                        break;
//                    case SD LK_f:
//                        return input_t::toggle_fullscreen;
                }
            }
        }
        return r;
    }
}

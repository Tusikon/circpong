//
// Created by Avikam Agur on 10/11/2017.
//

#ifndef CIRPONG_GAME_H
#define CIRPONG_GAME_H

#include "src/state.h"

#include "src/environment.h"
#include "src/start_game.h"
#include "src/scenes/scene.h"
#include "src/scenes/arena.h"

namespace pong {
    class game {
    private:
        environment env_;
        start_game start_game_;
        scene* scene_;
        arena* arena_;

        bool exit_;
        SDL_Window * window;
        SDL_GLContext maincontext; /* Our opengl context handle */

        void render();

        state s;
    public:
        game();
        ~game();

        void play();
    };
}

#endif //CIRPONG_GAME_H

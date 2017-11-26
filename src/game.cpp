//
// Created by Avikam Agur on 10/11/2017.
//

#include "src/game.h"

namespace pong {
    game::game() :
            exit_ {false},
            scene_ {nullptr},
            start_game_ {&scene_}
    {
        // Intialize SDL.
        SDL_Init(SDL_INIT_EVERYTHING);

        // Don't show cursor.
        SDL_ShowCursor(0);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        /* Turn on double buffering with a 24bit Z buffer.
         * You may need to change this to 16 or 32 for your system */
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);


        // Create window and renderer.
        window = SDL_CreateWindow("Pong",
                                  SDL_WINDOWPOS_UNDEFINED,  // Centered window.
                                  SDL_WINDOWPOS_UNDEFINED,  // Centered window.
                                  constants::screen_width, constants::screen_height,
                                  SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

        maincontext = SDL_GL_CreateContext(window);
        if (maincontext == nullptr) {
            std::cout << "Error initing opengl context " << SDL_GetError() << std::endl;
            throw std::runtime_error("maincontext init error");
        }
        printf("OpenGL version is (%s)\n", glGetString(GL_VERSION));

        // Needs to be initialized after we have an OpenGl context
        scene_ = new scene{};
        arena_ = new arena{};
    }

    game::~game() {
        SDL_GL_DeleteContext(maincontext);
        delete scene_;
        delete arena_;
        // Destroy  window.
        SDL_DestroyWindow(window);

        // Shuts down SDL.
        SDL_Quit();
    }

    void game::render() {
        SDL_GL_SetSwapInterval(1);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        scene_->render(s);
        arena_->render(s);

        std::cout << s.player_1_pos_xy[0].first << ", " << s.player_1_pos_xy[0].second << std::endl;
        s.player_1_ang++;

        /* Swap our back buffer to the front */
        SDL_GL_SwapWindow(window);
    }

    void game::play() {

        start_game_.render();
        while (env_.is_active()) {
            render();
            env_.update();

            SDL_Delay(25);

            env_.get_event();
        }
    }
}
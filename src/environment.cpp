//
// Created by Avikam Agur on 10/11/2017.
//

#include <SDL_opengl.h>
#include "src/constants.h"
#include "src/environment.h"

namespace pong{
    static int SCREEN_WIDTH = 720;
    static int SCREEN_HEIGHT = 720;

    static float radius = 1;


    environment::environment() :
            tick {0},
            unit {constants::PI / 50},
            p1 { 0, true, unit},
            p2 { constants::PI, false, unit}
    {
        // Intialize SDL.
        SDL_Init(SDL_INIT_EVERYTHING);

        // Don't show cursor.
        SDL_ShowCursor(0);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

        /* Turn on double buffering with a 24bit Z buffer.
         * You may need to change this to 16 or 32 for your system */
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);


        // Create window and renderer.
        window = SDL_CreateWindow("Pong",
                                  SDL_WINDOWPOS_UNDEFINED,  // Centered window.
                                  SDL_WINDOWPOS_UNDEFINED,  // Centered window.
                                  SCREEN_WIDTH,
                                  SCREEN_HEIGHT,
                                  SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

        maincontext = SDL_GL_CreateContext(window);

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                                  SDL_RENDERER_PRESENTVSYNC);

        // Instantiate game objects.

    }

    environment::~environment() {
        // Destroy renderer and window.
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);

        // Shuts down SDL.
        SDL_Quit();
    }

    void environment::render() {
        SDL_GL_SetSwapInterval(1);

        /* Clear our buffer with a red background */
        glClearColor ( 0.0, 0.0, 0.0, .3 );
        glClear ( GL_COLOR_BUFFER_BIT );

        glLoadIdentity();
        // render scene
        glBegin( GL_LINE_LOOP );
        for(double i = 0; i < 2 * constants::PI * radius; i += unit)
            glVertex2d(cos(i) * radius, sin(i) * radius);
        glEnd();

        glLoadIdentity();
        render(p1);
        render(p2);

        glLoadIdentity();
        render(b);

        /* Swap our back buffer to the front */
        SDL_GL_SwapWindow(window);
    }

    void environment::render(const ball &b) {
        double dist;
        double theta;
        b.get_location(&dist, &theta);
        auto x = dist * cos(theta);
        auto y = dist * sin(theta);

        if (x*x + y*y < (radius-0.055)*(radius-0.055)) {
            tick++;
        }

        glTranslated(x, y, 0);
        glBegin( GL_POLYGON );
        for(double i = 0; i < 2 * constants::PI * radius; i += constants::PI / 6) {
            glVertex2d(cos(i) * radius * 0.01, sin(i) * radius * 0.01);
        }
        glEnd();
    }

    void environment::render(const player& p) {
        //////
        double o = p.get_origin();

        double arc_size = (2 * constants::PI * radius) / 10;
        auto edge_points = (unsigned int) floor(arc_size / unit);

        glBegin( GL_TRIANGLE_STRIP );
        for(int i = 0; i <= 2*edge_points + 1; i++) {
            double theta = o + (unit * (i >> 1));

            if (i % 2 == 0)
                glVertex2d(cos(theta) * (radius - 0.01), sin(theta) * (radius - 0.01));
            else
                glVertex2d(cos(theta) * (radius - 0.04), sin(theta) * (radius - 0.04));
        }
        glEnd();
    }

    void environment::frame_delay() {
        SDL_Delay(25);
    }

    void environment::update() {
        b.move();
    }

    bool environment::get_event() {
        // Handle events.
        SDL_Event event {};
        while (SDL_PollEvent(&event)) {
            // Track mouse movement.
            if (event.type == SDL_MOUSEMOTION) {
                int mouse_x;
                //SDL_GetMouseState(&mouse_x, &tick);
            }

            // Clicking 'x' or pressing F4.
            if (event.type == SDL_QUIT) {
                return true;
            }

//            // Joystick direction controller moved.
//            if (event.type == SDL_JOYAXISMOTION) {
//                // 32767.
//                // Up or down.
//                if (event.jaxis.axis == 1) {
//                    gamepad_direction = event.jaxis.value/5461;
//                }
//            }

            // Joystick action button pressed.
//            if (event.type == SDL_JOYBUTTONDOWN) {
//                if (ball->status == ball->READY) {
//                    ball->status = ball->LAUNCH;
//                }
//            }

            // Pressing a key.
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    // Pressing ESC exits from the game.
                    case SDLK_ESCAPE:
                        return true;

                        // Pressing space will launch the ball if it isn't
                        // already launched.
                    case SDLK_SPACE:
//                        if (ball->status == ball->READY) {
//                            ball->status = ball->LAUNCH;
//                        }
                        break;

                    case SDLK_UP:
                        p1.go_up();
                        break;
                    case SDLK_DOWN:
                        p1.go_down();
                        break;

                    case SDLK_q:
                        p2.go_up();
                        break;
                    case SDLK_a:
                        p2.go_down();
                        break;

                        // Pressing F11 to toggle fullscreen.
//                    case SDLK_F11:
//                        int flags = SDL_GetWindowFlags(window);
//                        if (flags & SDL_WINDOW_FULLSCREEN) {
//                            SDL_SetWindowFullscreen(window, 0);
//                        } else {
//                            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
//                        }
//                        break;
                    default:
                        break;
                }
            }
        }
        return false;
    }
}

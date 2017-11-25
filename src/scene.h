//
// Created by Avikam Agur on 24/11/2017.
//

#ifndef CIRPONG_SCENE_H
#define CIRPONG_SCENE_H

#include <OpenGl/gl3.h>

namespace pong{
    class scene {
        GLuint shaderProgram;
        GLuint fragmentShader;
        GLuint vertexShader;
    public:
        scene();
        ~scene();
        void draw_texture(const GLvoid *pixels, int width, int height);
        void render();
    };
}

#endif //CIRPONG_SCENE_H
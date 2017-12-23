//
// Created by Avikam Agur on 22/12/2017.
//

#ifndef CIRPONG_CONFIG_H
#define CIRPONG_CONFIG_H

#include "src/constants.h"

namespace pong {
    struct config {
        int max_seconds_idle;
        int max_seconds_idle_instruction;
        int max_score;
        float ball_size;

        config(
                int max_seconds_idle = constants::max_seconds_idle,
                int max_seconds_idle_instruction = constants::max_seconds_idle,
                int max_score = constants::max_score,
                float ball_size = constants::ball_size

        ) :
            max_seconds_idle { max_seconds_idle },
            max_seconds_idle_instruction { max_seconds_idle_instruction },
            max_score{ max_score },
            ball_size{ ball_size }
                {}
    };

}

#endif //CIRPONG_CONFIG_H

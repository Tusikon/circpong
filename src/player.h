//
// Created by Avikam Agur on 11/11/2017.
//

#ifndef CIRPONG_PLAYER_H
#define CIRPONG_PLAYER_H

#include <string>
#include "src/constants.h"

namespace pong {
    struct player {
        float angle_;
        // how many angles are change on player movement (0-360)
        float angular_speed;
        int score;
        bool active;
        std::string name;
        std::pair<float,float> boundaries;


        explicit player(std::string name, float angle);

        void go_up();
        void go_down();
    };
}

#endif //CIRPONG_PLAYER_H

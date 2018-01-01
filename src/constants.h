//
// Created by Avikam Agur on 11/11/2017.
//

#ifndef CIRPONG_CONSTANTS_H
#define CIRPONG_CONSTANTS_H

#include <vector>
#include <tuple>

namespace pong {
    using pos_t = std::pair<float, float>;
    // (x,y) for each center of pixel. there are 2 pixels in every level and 1 as the base.
    using player_pos_t = std::vector<pos_t>;

    namespace constants {
        // arena radius
        static float PI = 3.14159265358;
        static float radius = 1.0f;

        // TODO: Consider calculating by player speed or other player position
        static int collision_cooldown_max_val = 10;

        static int screen_width = 768;
        static int screen_height = 768;

        static float player_size = 0.03;
        static float ball_size = 0.04;
        constexpr unsigned player_pixel_levels = 3;
        static float initial_velocity = 0.015;

        static float hit_threshold = 0.009;

        static int max_score = 3;

        static int start_game_counter = 3;

        static bool always_draw_scores = true;

        static int max_seconds_idle_instruction = 20;
        static int max_seconds_idle = 60;
        static int max_seconds_game_over = 5;
    }
}

#endif //CIRPONG_CONSTANTS_H

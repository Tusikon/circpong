//
// Created by Avikam Agur on 25/11/2017.
//

#ifndef CIRPONG_STATE_H
#define CIRPONG_STATE_H

#include "src/config.h"
#include "src/constants.h"
#include "src/environment.h"
#include "src/player.h"
#include <vector>
#include <tuple>
#include <random>
#include <chrono>

using namespace std::chrono;


namespace pong {
    class state {
        /*
         * return nullptr if no goal occurred or player scored the goal.
         */
        player* test_goal();

        inline void reset_ball() {
            float angle;
            if (winner == nullptr)
                angle = 90;
            else
                angle = winner->angle_;
            float start_angle = angle + (ceilf(uni(rng)/181)*2-1)*(10 + abs(uni(rng))/9);
            start_angle = start_angle + 360*(start_angle<0);
            if ((start_angle < 180 & angle > 180) | (start_angle > 180 & angle < 180))
                start_angle = angle-(start_angle - angle);
            ball_speed_x = float(_conf.initial_velocity * cos(start_angle * constants::PI/180));
            ball_speed_y = float(_conf.initial_velocity * sin(start_angle * constants::PI/180));
            ball_pos = {0, 0};
        }

        bool is_ball_player_collision(float angle);
        void hit(float p_angle);
        std::random_device rd;     // only used once to initialise (seed) engine
        std::mt19937_64 rng;    // random-number engine used (Mersenne-Twister in this case)
        std::uniform_int_distribution<int> uni; // guaranteed unbiased

        const config& _conf;
    public:
        bool is_welcome;
        bool is_instructions;
        bool is_game_start;
        bool is_2_player;
        // After a goal
        bool is_paused;
        // User ask for pause
        bool is_player_pressed_paused;
        bool is_goal;
        bool is_game_over;

        float ball_speed_x;
        float ball_speed_y;
        pos_t ball_pos;

        high_resolution_clock::time_point game_start_time;
        high_resolution_clock::time_point last_input_time;
        high_resolution_clock::time_point next_screen_time;


        player p1;
        player p2;

        player* winner;         //winner of current round
        player* curr_winner;    //winner of entire game
        int pc_cooldown;
        // After detecting a collision we start this timer that is reduced every frame
        // in which collision detection is disabled so we won't have "collision loop"
        int collision_cooldown;

        std::chrono::seconds start_game_count_down;

        explicit state(const config& conf) :
            _conf { conf },
            is_welcome { true },
            is_2_player { false },
            is_instructions { false },
            is_game_start { false },
            is_paused { true  },
            is_player_pressed_paused { false },
            is_goal { false },
            is_game_over {false },
            p1("Player 1", 90),
            p2("Player 2", 90+180),
            curr_winner {nullptr},
            winner {nullptr},
            pc_cooldown {0},
            collision_cooldown { constants::collision_cooldown_max_val },
            rng { rd() },
            uni {-180,180},
            game_start_time { high_resolution_clock::now() },
            last_input_time { high_resolution_clock::now() },
            next_screen_time { high_resolution_clock::time_point::max() },
            start_game_count_down { 0 }
            {
                reset_ball();
            };

        void update(input_t);
        void update_AI();
        void update_ball();
    };
}

#endif //CIRPONG_STATE_H

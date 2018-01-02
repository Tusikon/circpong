//
// Created by Avikam Agur on 27/11/2017.
//

#include "src/state.h"

namespace pong {
    void state::update(input_t event) {
        auto now = high_resolution_clock::now();
        auto p1_pressed = input_is_pause_p1(event);
        auto p2_pressed = input_is_pause_p2(event);
        if (event != input_t::idle)
            last_input_time = now;
        else {
            if (duration_cast<seconds>(now - last_input_time).count() > _conf.max_seconds_idle) {
                last_input_time = now;

                is_welcome = true;
                is_instructions = false;
                is_paused = true;
                is_game_start = false;
                is_2_player = false;
                is_player_pressed_paused = false;

                is_game_over = false;
                curr_winner = nullptr;
                pc_cooldown = 4;
                p1.active = false;
                p2.active = false;
                p1.score = 0;
                p2.score = 0;
                p1.angle_ = 90;
                p2.angle_ = 90+180;
                reset_ball();
            }
        }


        is_goal = false;

        if (is_welcome) {
            if (p1_pressed || p2_pressed ) {
                is_welcome = true;
                is_instructions = false;
                is_paused = true;
                is_game_start = false;
                p1.active |= p1_pressed;
                p2.active |= p2_pressed;
                is_2_player = p1.active && p2.active;
                next_screen_time = now + std::chrono::seconds(1);
            }
            else if (next_screen_time < now) {
                is_welcome = false;
                is_instructions = true;
                is_paused = true;
                is_game_start = false;
                p1.active |= p1_pressed;
                p2.active |= p2_pressed;
                is_2_player = p1.active && p2.active;
                next_screen_time = high_resolution_clock::time_point::max();
            }
            return;
        }

        if (is_instructions) {
            if ((event == input_t::idle) && !(next_screen_time < now)) {
                if (duration_cast<seconds>(now - last_input_time).count() > _conf.max_seconds_idle_instruction) {
                    is_welcome = true;
                    is_instructions = false;
                    is_paused = true;
                    is_game_start = false;
                    is_player_pressed_paused = false;
                    is_2_player = false;
                    curr_winner = nullptr;

                    p1.active = false;
                    p2.active = false;
                    p1.score = 0;
                    p2.score = 0;
                    p1.angle_ = 90;
                    p2.angle_ = 90+180;
                    reset_ball();
                }
            }
            else if ((p1_pressed && !p1.active) ||  (p2_pressed && !p2.active)) {
                is_welcome = false;
                is_instructions = true;
                is_paused = true;
                is_game_start = false;
                p1.active |= p1_pressed;
                p2.active |= p2_pressed;
                is_2_player = p1.active && p2.active;
                next_screen_time = now + std::chrono::seconds(1);
            }
            else if ((p1_pressed && p1.active) || (p2_pressed && p2.active) || (next_screen_time < now) ) {
                is_welcome = false;
                is_instructions = false;
                is_paused = true;
                is_game_start = true;
                game_start_time = high_resolution_clock::now();
                p1.active |= p1_pressed;
                p2.active |= p2_pressed;
                is_2_player = p1.active && p2.active;
                next_screen_time = high_resolution_clock::time_point::max();
            }

            return;
        }


        if (is_game_start) {
            start_game_count_down = duration_cast<seconds>(now - game_start_time );
            if (start_game_count_down.count() >= constants::start_game_counter) {
                is_game_start = false;
                is_paused = false;
            } else {
                return;
            }
        }

        if (is_game_over) {
            if ((now > next_screen_time)) {
                is_game_over = false;
                p1.score = 0;
                p2.score = 0;
                p1.active = false;
                p1.active = false;
                curr_winner = nullptr;
                is_2_player = false;

                p1.angle_ = 90;
                p2.angle_ = 90+180;

                is_paused = true;
                is_player_pressed_paused = false;
                is_game_start = false;
                is_welcome = true;
                next_screen_time = high_resolution_clock::time_point::max();
            } else{
                input_is_pause_p1(event);
                input_is_pause_p2(event);
            }
                return;
        }

        if (input_is_pause_p1(event) || input_is_pause_p2(event)) {
            // If the game wasn't paused and a pause input has come -> player pressed pause is true to indicate
            // that a "RESUME" statement should be displayed.
            is_player_pressed_paused = !is_paused;

            is_paused = !is_paused;
        }

        if (is_paused)
            return;

        if (input_is_set(event, input_t::player_1_up) && (p1.active  || is_2_player) ) {
            p1.go_up();
        }
        if (input_is_set(event, input_t::player_1_down) && (p1.active  || is_2_player) ) {
            p1.go_down();
        }

        if (input_is_set(event, input_t::player_2_up)  && (p2.active  || is_2_player) ) {
            p2.go_up();
        }
        if (input_is_set(event, input_t::player_2_down) && (p2.active  || is_2_player) ) {
            p2.go_down();
        }
        p1.active |= p1_pressed;
        p2.active |= p2_pressed;
        is_2_player = p1.active && p2.active;


    }

    void state::update_AI() {
        if (!is_2_player & !pc_cooldown & !is_paused){
            pc_cooldown = 4;
            auto ball_next_pos_x = ball_pos.first , ball_next_pos_y = ball_pos.second;
            float Dist=-1;
            auto Itr = 0 ;
            while (abs(Dist) > 0.01 & Itr < 15) {
                ball_next_pos_x += ((Dist<0)*2-1)*ball_speed_x*512/pow(2,Itr);
                ball_next_pos_y += ((Dist<0)*2-1)*ball_speed_y*512/pow(2,Itr);
                Dist = float(pow(ball_next_pos_x,2) + pow(ball_next_pos_y,2) - 1);
                Itr++;
            }
            auto ball_theta = (atan2(ball_next_pos_y , ball_next_pos_x ) * 180 / constants::PI);
            ball_theta += 360*(ball_theta<0);
            if (p1.active){
                if (ball_theta > 180)
                    if  (p2.angle_ - p2.angular_speed/2 > ball_theta | p2.angle_ + p2.angular_speed/2 < ball_theta){
                        if (ball_theta  > p2.angle_)
                            p2.go_up();
                        else if (p2.angle_> ball_theta)
                            p2.go_down(); }}
            else if(p2.active){
                if (ball_theta < 180)
                    if  (p1.angle_ - p1.angular_speed/2 > ball_theta | p1.angle_ + p1.angular_speed/2 < ball_theta){
                        if (ball_theta  > p1.angle_)
                            p1.go_up();
                        else if (p1.angle_> ball_theta)
                            p1.go_down(); }}
        }
        else if (!is_2_player & pc_cooldown!=0)
            pc_cooldown = (pc_cooldown <= 0) ? 0 : pc_cooldown-1;
    }

    void state::update_ball() {
        if (is_paused)
            return;

        ball_pos.first  += ball_speed_x;
        ball_pos.second += ball_speed_y;

        if (collision_cooldown > 0) {
            collision_cooldown -= 1;
        } else {
            if (ball_pos.second >= 0 && is_ball_player_collision(p1.angle_)) {
                std::cout << "player 1 collision\n";
                hit(p1.angle_);
            }

            if (ball_pos.second <= 0 &&  is_ball_player_collision(p2.angle_ - 180)) {
                std::cout << "player 2 collision\n";
                hit(p2.angle_ - 180);
            }


        }

        winner = test_goal();
        if (winner != nullptr) {
            std::cout << "goal!" << std::endl;
            is_paused = true;
            is_player_pressed_paused = false;
            is_goal = true;

            reset_ball();
            collision_cooldown = 0;

            winner->score += 1;

            if (winner->score == _conf.max_score) {
                is_game_over = true;
                curr_winner = winner;
                next_screen_time = high_resolution_clock::now() + std::chrono::seconds(constants::max_seconds_game_over);
            }
        }
    }

    bool state::is_ball_player_collision(float player_theta) {
        auto ball_theta = (atan(ball_pos.second / ball_pos.first ) * 180 / constants::PI);
        if (ball_pos.first * ball_pos.second < 0) {
            ball_theta += 180;
        }

        auto effective_radius = _conf.radius * 0.85;
        if (pow(ball_pos.first, 2) + pow(ball_pos.second, 2) >= pow(effective_radius, 2)) {
            if (player_theta - 6.5 <= ball_theta && ball_theta <= player_theta + 6.5)
                return true;
        }
        return false;
    }

    player* state::test_goal() {
        // Factor the size of the paddle in the hit calculation
        auto effective_radius = _conf.radius * 0.95;
        if (pow(ball_pos.first, 2) + pow(ball_pos.second, 2) <= pow(effective_radius,2) ) {
            return nullptr;
        }

        // check x coordinate (first coordinate) of the ball being to the left or to the right of the center
        if (ball_pos.second > 0) {
            return &p2;
        }

        return &p1;
    }

    void state::hit(float p_angle) {
        collision_cooldown = constants::collision_cooldown_max_val;
        auto ball_theta = (atan(ball_pos.second / ball_pos.first ) * 180 / constants::PI);
        if (ball_pos.first * ball_pos.second < 0) {
            ball_theta += 180;
        }
        auto Angle_shift = uni(rng)/90 + 2*(ball_theta - p_angle);
        // rotate speed vector by random angle
        float shift_cos = cos(Angle_shift * constants::PI/180);
        float shift_sin = sin(Angle_shift * constants::PI/180);

        // change speed
        ball_speed_y = (ball_speed_y * 1.1 < _conf.radius*0.3) ? ball_speed_y * 1.05 : ball_speed_y;
        ball_speed_x = (ball_speed_x * 1.1 < _conf.radius*0.3) ? ball_speed_x * 1.05 : ball_speed_x;

        // change direction
        auto speed_x_tmp = ball_speed_x*shift_cos - ball_speed_y*shift_sin;
        ball_speed_y = -1 * (ball_speed_x*shift_sin + ball_speed_y*shift_cos);
        ball_speed_x = -1 * speed_x_tmp;
    }
}

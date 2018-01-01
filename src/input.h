//
// Created by pong on 16/12/17.
//

#ifndef CIRPONG_INPUT_H
#define CIRPONG_INPUT_H

#include <type_traits>

namespace pong {
    enum class input_t : int {
        // no input between frames
        idle                = 0,
        player_1_up         = 0x0001,
        player_1_down       = 0x0002,
        player_2_up         = 0x0004,
        player_2_down       = 0x0008,

        toggle_fullscreen   = 0x0010,

        p1_press            = 0x0100,
        p2_press            = 0x0200,

        screen_left         = 0x0400,
        screen_right        = 0x0800,
        screen_up           = 0x1000,
        screen_down         = 0x2000,
        screen_decr         = 0x4000,
        screen_incr         = 0x8000,

        quit                = 0x10000
    };

    using T = std::underlying_type_t <input_t>;

    inline input_t operator| (input_t lhs, input_t rhs)
    {
        return (input_t)(static_cast<T>(lhs) | static_cast<T>(rhs));
    }

    inline input_t& operator|= (input_t& lhs, input_t rhs)
    {
        lhs = (input_t)(static_cast<T>(lhs) | static_cast<T>(rhs));
        return lhs;
    }

    inline input_t operator& (input_t lhs, input_t rhs)
    {
        return (input_t)(static_cast<T>(lhs) & static_cast<T>(rhs));
    }

    inline bool input_is_set(const input_t val, const input_t flag) {
        return input_t::idle != (val & flag);
    }

    inline bool input_is_pause_p1(const input_t val) {
        return input_is_set(val, input_t::p1_press);
    }

    inline bool input_is_pause_p2(const input_t val) {
        return input_is_set(val, input_t::p2_press);
    }

    inline bool input_all_pressed(const input_t val) {
        return input_is_set(val, input_t::p1_press) && input_is_set(val, input_t::p2_press);
    }
}

#endif //CIRPONG_INPUT_H

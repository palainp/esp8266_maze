#ifndef PLAYER_H
#define PLAYER_H

#ifdef __XTENSA__
#include <Arduino.h>
#endif

#include "utils.hpp"


class Player
{
public:
    Player(){};
    ~Player(){};

    void clear();
    void teleport(int32_t nx, int32_t ny);
    void move(t_direction d);
    uint8_t red_color() {return led_color>>16 & 0xFF;};
    uint8_t green_color() {return led_color>>8 & 0xFF;};
    uint8_t blue_color() {return led_color & 0xFF;};

    uint32_t x, y;
    uint32_t status;
    uint32_t led_color;

};

#endif

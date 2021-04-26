#include "Player.hpp"

void Player::clear(){
    status=led_color=0;
};

void Player::teleport(int32_t nx, int32_t ny)
{
    x=nx;
    y=ny;
}

void Player::move(t_direction d)
{
    switch(d)
    {
        case N:
            --x;
            break;
        case S:
            ++x;
            break;
        case W:
            --y;
            break;
        case E:
            ++y;
            break;
        default:
            break;
    }
};

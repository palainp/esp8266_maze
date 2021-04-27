#ifndef HELPER_H
#define HELPER_H
#include "Item.hpp"
#include "Compass.hpp"

class Helper:public Item
{
    public:
    Helper(int32_t x, int32_t y, bool is_liar, Compass *c):lie(is_liar),Item(x,y){
        cell_text = {
            "You found someone !",
            "You can hear someone speaking in the direction ",
            "The legend says that someone lives in the ",
        };
        cell_distance = {0, 4*4*2, 7*7*2}; // we have to hear the helper very far to really help the player
        cell_show_direction = {false, true, true};
        // here we want to appeal the player with a randomly shorter fake distance :)
        if(is_liar) {
            cx = x+rand()%20-10;
            cy = y+rand()%20-10;
        } else {
            cx = c->x;
            cy = c->y;
        }
    };

    void move(uint8_t cell) {
        // roughly 50% percent of moving, and 12.5% in each direction (if open)
        switch(rand()%8){
            case 0: //N
                if(cell&N) --x;
                break;
            case 1: //S
                if(cell&S) ++x;
                break;
            case 2: //E
                if(cell&E) ++y;
                break;
            case 3: //W
                if(cell&W) --y;
                break;
            default: // not moving
                break;
        }
    };

    void update_status(Player &p) {
        if (distance(p)==0)
        {
            if (lie) {
                p.led_color = 0xFF0000;
            } else {
                p.led_color = 0x00FF00;
            }
        }
    };

    std::string display_cell(Player &p) {
#ifdef DEBUG
        std::string str = " Helper(";
        if (lie) str+=SSTR("false): ");
        else  str+=SSTR("true): ");
        str += SSTR(x << ","<< y);
        return str;
#else
        if (distance(p)==0){
            std::string str = " I think I wil go to ";
            str += SSTR(cx << ","<< cy);
            return str;
        } else {
            return "";
        }
#endif
    };

    private:
        bool lie;
        int32_t cx,cy;
};

#endif

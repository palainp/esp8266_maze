#ifndef HELPER_H
#define HELPER_H
#include "Item.hpp"
#include "Compass.hpp"

class Helper:public Item
{
    public:
    Helper(int32_t x, int32_t y, bool is_liar, Compass *c):lie(is_liar),cx(c->x),cy(c->y),Item(x,y){
        cell_text = {
            "You found someone !",
            "You can ear someone speeking in the direction ",
        };
        cell_distance = {0, 18};
        cell_show_direction = {false, true};
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

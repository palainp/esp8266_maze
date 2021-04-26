#ifndef COMPASS_H
#define COMPASS_H
#include "Item.hpp"

class Compass:public Item
{
    public:
    Compass(int32_t x, int32_t y):Item(x,y){
        cell_text = {
            "You just found the compass !",
            "You can see something shinning in the direction ",
            "A small noise came from the direction ",
            "You can hear a metallic object fall to the ground near here."
        };
        cell_distance = {0, 3, 8, 18};
        cell_show_direction = {false, true, true, false};
    };

    void update_status(Player &p) {
        if (distance(p)==0) {
            p.status |= got_compass;
            p.led_color = 0xFFFF00;
        }
    };

    std::string display_cell(Player &p) {
#ifdef DEBUG
        std::string str = " Compass: ";
        str += SSTR(x << ","<< y);
        return str;
#else
        return Item::display_cell(p);
#endif
    };

    std::string display_text(Player &p) {
        if (!(p.status & got_compass) || distance(p)==0)
            return Item::display_text(p);
        else return "";
    };

};

#endif

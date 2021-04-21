#ifndef COMPASS_H
#define COMPASS_H
#include "Item.hpp"

class Compass:public Item
{
    public:
    Compass(int32_t x, int32_t y):Item(x,y){
        cell_text = {
            "You just found the compass !",
            "Vous voyez un reflet dans la direction ",
            "Un léger bruit venait de la direction ",
            "Vous entendez quelque chose de métalique tomber au sol pas très loin."
        };
        cell_distance = {0, 3, 8, 18};
        cell_show_direction = {false, true, true, false};
    };

    void update_status(int32_t x, int32_t y, uint32_t &status) {
        if (distance(x,y)==0) status |= got_compass;
    };

    std::string display_text(int32_t x, int32_t y, uint32_t &status) {
        if (!(status & got_compass) || distance(x,y)==0)
            return Item::display_text(x,y,status);
        else return "";
    };

};

#endif

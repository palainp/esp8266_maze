#ifndef EXIT_H
#define EXIT_H
#include "Item.hpp"

class Exit:public Item
{
    public:
    Exit(int32_t x, int32_t y):Item(x,y){
        cell_text = {
                "You just found the exit.... or this is just the starting point for another run ??",
                "The exit is really near.",
                "Vous arrivez presque au but !"
        };
        cell_distance = {0, 8, 18};
    };

    void update_status(int32_t x, int32_t y, uint32_t &status) {
        if(distance(x,y)==0) status &= exit_found;
    };

};

#endif

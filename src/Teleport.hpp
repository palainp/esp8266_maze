#ifndef TELEPORT_H
#define TELEPORT_H
#include "Item.hpp"

class Teleport:public Item
{
    public:
    Teleport(int32_t x, int32_t y):Item(x,y){
        cell_text = {
            "Something buuzzzz in the ",
        };
        cell_distance = {4};
        cell_show_direction = {true};
    };

    std::string display_text(Player &p) {
        std::string str = "";
        if (distance(p)==0) {
            str += SSTR("                   .-'''-.        .-'''-.                    \r\n");
            str += SSTR("                 '   _    \\     '   _    \\                   \r\n");
            str += SSTR("               /   /` '.   \\  /   /` '.   \\      .           \r\n");
            str += SSTR("       _     _.   |     \\  ' .   |     \\  '    .'|           \r\n");
            str += SSTR(" /\\    \\   //|   '      |  '|   '      |  '  <  |           \r\n");
            str += SSTR(" `\\  //\\ // \\    \\     / / \\    \\     / /    | |           \r\n");
            str += SSTR("   \\`//  \'/   `.   ` ..' /   `.   ` ..' / _   | | .'''-.    \r\n");
            str += SSTR("    \\|   |/       '-...-'`       '-...-'`.' |  | |/.'''. \\   \r\n");
            str += SSTR("     '                                  .   | /|  /    | |   \r\n");
            str += SSTR("                                      .'.'| |//| |     | |   \r\n");
            str += SSTR("                                    .'.'.-'  / | |     | |   \r\n");
            str += SSTR("                                    .'   \\_.'  | '.    | '.  \r\n");
            str += SSTR("                                               '---'   '---' \r\n");
            // we must have random teleport as unfortunatly one can be on the only way to the exit...
            p.teleport(x+rand()%20-10,y+rand()%20-10);
        } else {
            return Item::display_text(p);
        }
        return str;
    };


    std::string display_cell(Player &p) {
#ifdef DEBUG
        std::string str = " Teleport: ";
        str += SSTR(x << ","<< y);
        return str;
#else
        return "";
#endif
    };
};

#endif

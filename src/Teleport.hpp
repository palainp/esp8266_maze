#ifndef TELEPORT_H
#define TELEPORT_H
#include "Item.hpp"

class Teleport:public Item
{
    public:
    Teleport(int32_t x, int32_t y):Item(x,y){
        cx = x+rand()%20-10;
        cy = y+rand()%20-10;
    };

    std::string display_text(Player &p) {
        std::string str = "";
        if (distance(p)==0) {
            str += SSTR("                          .-'''-.        .-'''-.             \r\n");
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
            p.teleport(cx,cy);
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

    private:
        int32_t cx,cy;
};

#endif

#ifndef GHOST_H
#define GHOST_H
#include "Item.hpp"

class Ghost:public Item
{
    public:
    Ghost(int32_t x, int32_t y):Item(x,y){
        cell_text = {
            "I can smmelll you coward !",
            "some cold wind comes from ",
        };
        cell_distance = {4*4*2, 7*7*2};
        cell_show_direction = {false, true};
    };

    void update_status(Player &p) {
        if (distance(p)<=cell_distance[0]) {
            // linear color from 00 to FF on the blue part
            uint8_t blue = p.led_color & 0xFF;
            uint8_t color = 254*((cell_distance[0]-distance(p))/cell_distance[0]);
            if (color>blue) p.led_color |= color;
        }
    };

    void move(Player &p, uint8_t cell) {
        // roughly 30% percent of moving in the direction of the player (even if there is a wall)
        // and 10% in each open direction
        switch(rand()%10){
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
            case 4:
            case 5:
            case 6:
                if (x<p.x) ++x;
                else if (x>p.x) --x;
                if (y<p.y) ++y;
                else if (y>p.y) --y;
            default: // not moving
                break;
        }
    };

    std::string display_text(Player &p) {
        std::string str = "";
        if (distance(p)==0) {
            str += SSTR("              .-'''-.           \r\n");
            str += SSTR("             '   _    \\         \r\n");
            str += SSTR("   .       /   /` '.   \\        \r\n");
            str += SSTR(" .'|      .   |     \\  '        \r\n");
            str += SSTR("<  |      |   '      |  '       \r\n");
            str += SSTR(" | |      \\    \\     / /        \r\n");
            str += SSTR(" | | .'''-.`.   ` ..' /_    _   \r\n");
            str += SSTR(" | |/.'''. \\  '-...-'`| '  / |  \r\n");
            str += SSTR(" |  /    | |         .' | .' |  \r\n");
            str += SSTR(" | |     | |         /  | /  |  \r\n");
            str += SSTR(" | |     | |        |   `'.  |  \r\n");
            str += SSTR(" | '.    | '.       '   .'|  '/ \r\n");
            str += SSTR(" '---'   '---'       `-'  `--'  \r\n");

            // teleport the player and move away !
            p.teleport(x+rand()%20-10,y+rand()%20-10);
            x = x+rand()%60-30;
            y = y+rand()%60-30;
        } else {
            return Item::display_text(p);
        }
        return str;
    };


    std::string display_cell(Player &p) {
#ifdef DEBUG
        std::string str = " Ghost: ";
        str += SSTR(x << ","<< y);
        return str;
#else
        return "";
#endif
    };
};

#endif

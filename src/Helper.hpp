#ifndef HELPER_H
#define HELPER_H
#include "Item.hpp"
#include "Compass.hpp"

class Helper:public Item
{
    public:
    Helper(int32_t x, int32_t y):lie_(true),Item(x,y){
        cell_text = {
            "You found someone !",
            "You can hear someone speaking in the direction ",
            "The legend says that someone lives in the ",
        };
        cell_distance = {0, 4*4*2, 7*7*2}; // we have to hear the helper very far to really help the player
        cell_show_direction = {false, true, true};
        // here we want to appeal the player with a randomly shorter fake distance :)
        cx_ = x+rand()%20-10;
        cy_ = y+rand()%20-10;
    };

    Helper(int32_t x, int32_t y, int32_t cx, int32_t cy):lie_(false),cx_(cx),cy_(cy),Item(x,y){
        cell_text = {
            "You found someone !",
            "You can hear someone speaking in the direction ",
            "The legend says that someone lives in the ",
        };
        cell_distance = {0, 4*4*2, 7*7*2}; // we have to hear the helper very far to really help the player
        cell_show_direction = {false, true, true};
    };

    void move(Player &p, uint8_t cell) {
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
            if (lie_) {
                p.led_color = 0xFF0000;
            } else {
                p.led_color = 0x00FF00;
            }
        }
    };
    std::string display_text(Player &p) {
        std::string str = "";
        if (distance(p)==0)
        {
            str += SSTR("A long time ago I found something (it buzzzed when I was around it) that wooshes me ");
            if (lie_) {
              str += SSTR("at least 50 cells away ! They are really really rare but if you found one you must avoid it...");
            } else {
              str += SSTR("at most 10 cells away ! I certainly know that there is 10 of them in the 20x20 square around the starting point...");
              str += SSTR("You also can see some ghosts that will bother you I think...");
            }
        }
        return str;
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
            str += SSTR(cx_ << ","<< cy_);
            return str;
        } else {
            return "";
        }
#endif
    };

    private:
        bool lie_;
        int32_t cx_,cy_;
};

#endif

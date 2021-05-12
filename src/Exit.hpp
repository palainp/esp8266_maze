#ifndef EXIT_H
#define EXIT_H
#include "Item.hpp"

class Exit:public Item
{
    public:
    Exit(int32_t x, int32_t y):Item(x,y){
        cell_text = {
                "You almost found the exit.... or this is just the starting point for another run ??",
                "The exit is really near.",
                "You almost reach the goal!"
        };
        cell_distance = {2, 8, 18};
        cell_show_direction = {false, false, false};
    };

    void update_status(Player &p) {
        if(distance(p)==0) p.status |= exit_found;
    };
    std::string display_text(Player &p) {
        std::string str = "";
        if (distance(p)==0) {
            str += SSTR(" .----------------.  .----------------.  .----------------.  .----------------. \r\n");
            str += SSTR("| .--------------. || .--------------. || .--------------. || .--------------. |\r\n");
            str += SSTR("| |  _________   | || |  ____  ____  | || |     _____    | || |  _________   | |\r\n");
            str += SSTR("| | |_   ___  |  | || | |_  _||_  _| | || |    |_   _|   | || | |  _   _  |  | |\r\n");
            str += SSTR("| |   | |_  \\_|  | || |   \\ \\  / /   | || |      | |     | || | |_/ | | \\_|  | |\r\n");
            str += SSTR("| |   |  _|  _   | || |    > `' <    | || |      | |     | || |     | |      | |\r\n");
            str += SSTR("| |  _| |___/ |  | || |  _/ /'`\\ \\_  | || |     _| |_    | || |    _| |_     | |\r\n");
            str += SSTR("| | |_________|  | || | |____||____| | || |    |_____|   | || |   |_____|    | |\r\n");
            str += SSTR("| |              | || |              | || |              | || |              | |\r\n");
            str += SSTR("| '--------------' || '--------------' || '--------------' || '--------------' |\r\n");
            str += SSTR("'----------------'  '----------------'  '----------------'  '----------------' \r\n");
        } else {
            return Item::display_text(p);
        }
        return str;
    };

    std::string display_cell(Player &p) {
#ifdef DEBUG
        std::string str = " Exit: ";
        str += SSTR(x << ","<< y);
        return str;
#else
        if ((p.status & got_compass) && !(p.status & exit_found))
        {
            std::string str = " Exit: ";
            str += SSTR(x << ","<< y);
            return str;
        }
        else return "";
#endif
    };


};

#endif

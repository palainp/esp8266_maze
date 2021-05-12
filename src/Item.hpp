#ifndef ITEM_H
#define ITEM_H
#include <string>
#include <vector>
#include "Player.hpp"
#include "utils.hpp"

typedef enum{
    got_compass=1U,
    exit_found=2U,
} t_status;

class Item
{
    public:

    int32_t x, y;

    Item(int32_t x, int32_t y):x(x),y(y){};
    ~Item(){};

    virtual std::string direction(Player &p)
    {
        std::string dx=(p.x>x?"N":(p.x<x?"S":" "));
        std::string dy=(p.y>y?"W":(p.y<y?"E":" "));
        return dx+dy;
    };

    virtual uint32_t distance(Player &p)
    {
        return ((p.x-x)*(p.x-x)+(p.y-y)*(p.y-y));
    };

    virtual std::string display_cell(Player &p) {
        return "";
    };

    virtual void move(Player &p, uint8_t cell) {};
    virtual void update_status(Player &p) {};
    
    virtual std::string display_text(Player &p) {
        size_t i=0;
        while(i<cell_distance.size() && distance(p)>cell_distance[i]) {++i;}
        if (i<cell_distance.size())
        {
            // assert i<cell_show_direction.size() && i<cell_text.size()
            if (cell_show_direction[i])
                return cell_text[i]+direction(p);
            else return cell_text[i];
        } else return "";
    };

    protected:
    // this will be printed each time we're at cell_distance from the Item
    std::vector<std::string> cell_text;
    std::vector<uint32_t> cell_distance;
    std::vector<bool> cell_show_direction;
};

#endif

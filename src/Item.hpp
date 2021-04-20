#ifndef ITEM_H
#define ITEM_H
#include <string>
#include <vector>

typedef enum{
    got_compass=1U,
    exit_found=2U,
} t_status;

class Item
{
    public:
    Item(int32_t x, int32_t y):ix(x),iy(y){};
    ~Item(){};

    virtual std::string direction(int32_t x, int32_t y)
    {
        std::string dx=(x>ix?"N":(x<ix?"S":" "));
        std::string dy=(y>iy?"W":(y<iy?"E":" "));
        return dx+dy;
    };

    virtual uint32_t distance(int32_t x, int32_t y)
    {
        return ((x-ix)*(x-ix)+(y-iy)*(y-iy));
    };

    virtual std::string display_cell(int32_t x, int32_t y) {
        return "";
    };
    virtual void update_status(int32_t x, int32_t y, uint32_t &status) {};
    virtual std::string display_text(int32_t x, int32_t y, uint32_t &status) {
        size_t i=0;
        while(i<cell_distance.size() && distance(x,y)>cell_distance[i]) {++i;}
        if (i<cell_distance.size()) return cell_text[i];
        else return "";
    };

    protected:
    int32_t ix, iy;
    // this will be printed each time we're at cell_distance from the Item
    std::vector<std::string> cell_text;
    std::vector<uint32_t> cell_distance;
};

#endif

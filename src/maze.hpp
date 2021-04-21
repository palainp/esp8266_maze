#ifndef MAZE_H
#define MAZE_H

#ifdef __XTENSA__
#include <Arduino.h>
#else
#include <stdlib.h>     /* srand, rand */
#endif

#include <vector>
#include <string>
#include <memory>

#include "Item.hpp"
#include "utils.hpp"

#define BUILD_INFO  "v0.2 " __DATE__
//#define DEBUG

static std::vector<std::string> logo ={
    " ______  __, _ __    _ _ _            ",
    "(  /    (   ( /  )  ( / ) )           ",
    "  /--    `.  /--'    / / / __,  __, _ ",
    "(/____/(___)/       / / (_(_/(_/_/_(/_",
    "                               (/     ",
    "                                      "
};

static std::vector<std::string> welcome = {
    "Bienvenue étranger, ce labyrinthe est désormais votre ultime demeure",
    "Vous avez le droit d'avoir peur et d'appuyer sur q pour faire appel aux dieux et sortir de là",
    "Si votre bravoure est suffisante, vous pourrez changer de pièce avec : ",
    "   i         8",
    " j k l     4 2 6",
    "Non loin de votre point d'arrivée vous pourrez trouver une boussole ce qui vous sera fort utile pour quêter la sortie...",
    "Enfin de sortie, ce n'est pas tant une sortie qu'un éternel recommencement..."
};

#define MAP_SIZE 100

// the 'direction bit' (i.e. NSEW as in t_cell) indicates if the wall is opened (1) or closed (0) 
#define set_open_(c,d) ((c)|=(d))
#define is_open_(c,d) ((c)&(d))

typedef enum{
    nothing=0,
    N=1U, S=2U, E=4U, W=8U,
} t_cell;

class Maze
{
public:
    Maze(){};
    ~Maze(){};
    bool is_in_map(int32_t x, int32_t y);
    bool is_set(int32_t x, int32_t y, t_cell d);
    void set_level(int32_t x, int32_t y);
    void load_map(int32_t x, int32_t y);
    void generate(int32_t x, int32_t y);
    std::string display_cell(int32_t x, int32_t y, uint32_t status);

    uint32_t x0,y0; // upper left absolute location
    uint8_t maze[MAP_SIZE][MAP_SIZE];
    std::vector<Item *> items;
    uint32_t status;

    // internal rand
    uint32_t seed;
    void set_rng(uint32_t s){seed=s; srand(seed);};
    void next_rng(){set_rng(seed+1);};
    void prev_rng(){set_rng(seed-1);};
    bool coinbias(double x){
#ifdef __XTENSA__
        yield();
#endif
        return(rand()<x*RAND_MAX);
    };
    bool coinflip(){return coinbias(0.5);};
};

#endif

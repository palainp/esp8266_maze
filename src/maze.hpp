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
#include "Player.hpp"
#include "utils.hpp"

static std::vector<std::string> logo ={
    " ______  __, _ __    _ _ _            ",
    "(  /    (   ( /  )  ( / ) )           ",
    "  /--    `.  /--'    / / / __,  __, _ ",
    "(/____/(___)/       / / (_(_/(_/_/_(/_",
    "                               (/     ",
    "                                      "
};

static std::vector<std::string> welcome = {
    "Welcome stranger, this labyrinth is now your final home",
    "You are right to be afraid (and you can press q to ask the gods for help and get out of there)",
    "If you have enough bravery, you can move with : ",
    "   i         8",
    " j k l     4 2 6",
    "You might come across people or items that you might find useful in finding the exit..."
};

#define MAP_SIZE 100

// the 'direction bit' (i.e. NSEW as in t_cell) indicates if the wall is opened (1) or closed (0) 
#define set_open_(c,d) ((c)|=(d))
#define is_open_(c,d) ((c)&(d))

class Maze
{
public:
    Maze(){};
    ~Maze(){};
    bool is_in_map(Player &p);
    bool is_set(uint8_t cell, t_direction d);
    uint8_t cell(int32_t x, int32_t y);
    void set_level(Player &p);
    void load_map(Player &p);
    void generate(Player &p);
    std::string display_cell(Player &p);

    uint32_t x0,y0; // upper left absolute location
    uint8_t maze[MAP_SIZE][MAP_SIZE];
    std::vector<Item *> items;

private:
    bool is_loaded(int32_t x,int32_t y);

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

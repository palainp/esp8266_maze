// g++ linux_maze.cpp src/*.cpp -Isrc -o maze
#include <iostream>
#include "maze.hpp"

uint32_t x, y;
Maze m;

void print_to_client(std::string str)
{
    if (str!="") std::cout << str << std::endl;
}

int main(int argc, char *argv[])
{
    bool print_maze = false;
    if (argc != 1)
    {
        print_maze = true;
    }

    if (!print_maze)
        std::cout << BUILD_INFO << std::endl;

    srand(0);
    x = rand() % 5000 + 1000;
    y = rand() % 5000 + 1000;
    if (!print_maze)
        std::cout << "Maze generation..." << std::endl;
    m.generate(x, y);
    if (!print_maze)
        std::cout << "done" << std::endl;

    if (print_maze)
    {
        for (auto i = 0; i < MAP_SIZE; ++i)
        {
            for (auto j = 0; j < MAP_SIZE; ++j)
            {
                int x = 0;
                x += (is_open_(m.maze[i][j], N) ? N : 0);
                x += (is_open_(m.maze[i][j], S) ? S : 0);
                x += (is_open_(m.maze[i][j], E) ? E : 0);
                x += (is_open_(m.maze[i][j], W) ? W : 0);

                printf("%2d ", x);
            }
            printf("\n");
        }
        exit(EXIT_SUCCESS);
    }

    for (std::string line : logo)
    {
        print_to_client(line);
    }
    for (std::string line : welcome)
    {
        print_to_client(line);
    }
    print_to_client(m.display_cell(x, y, m.status));

    while (true)
    {
        char c;
        std::cin >> c;
        if ((c == 'i' || c == '8') && m.is_set(x, y, N))
        { // to North and no wall
            x--;
        }
        else if ((c == 'k' || c == '2') && m.is_set(x, y, S))
        { // to South and no wall
            x++;
        }
        else if ((c == 'j' || c == '4') && m.is_set(x, y, W))
        { // to West and no wall
            y--;
        }
        else if ((c == 'l' || c == '6') && m.is_set(x, y, E))
        { // to East and no wall
            y++;
        }
        else if (c == 'q')
        { // to quit
            break;
        }
        else
        {
            continue;
        }

        if (!(m.is_in_map(x, y)))
        {
            m.load_map(x, y);
        }

        for (auto &i : m.items)
        {
            i->update_status(x, y, m.status);
        }
        for (auto &i : m.items)
        {
            print_to_client(i->display_text(x, y, m.status));
        }

        print_to_client(m.display_cell(x, y, m.status));
    }

    return EXIT_SUCCESS;
}

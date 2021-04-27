// g++ linux_maze.cpp src/*.cpp -Isrc -o maze
#include <iostream>
#include "maze.hpp"

Maze m;
Player p;

void print_to_client(std::string str)
{
    if (str!="") std::cout << str << std::endl;
}

void show_color(Player &p)
{
  printf("0x%02x%02x%02x\n", p.red_color(), p.green_color(), p.blue_color());
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
    p.teleport(rand() % 5000 + 1000, rand() % 5000 + 1000);

    if (!print_maze)
        std::cout << "Maze generation..." << std::endl;
    m.generate(p);
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
    print_to_client(m.display_cell(p));

    while (true)
    {
        char c;
        std::cin >> c;
        if ((c == 'i' || c == '8') && m.is_set(m.cell(p.x, p.y), N))
        { // to North and no wall
            p.move(N);
        }
        else if ((c == 'k' || c == '2') && m.is_set(m.cell(p.x, p.y), S))
        { // to South and no wall
            p.move(S);
        }
        else if ((c == 'j' || c == '4') && m.is_set(m.cell(p.x, p.y), W))
        { // to West and no wall
            p.move(W);
        }
        else if ((c == 'l' || c == '6') && m.is_set(m.cell(p.x, p.y), E))
        { // to East and no wall
            p.move(E);
        }
        else if (c == 'q')
        { // to quit
            break;
        }
        else
        {
            continue;
        }

        if (!(m.is_in_map(p)))
        {
            m.load_map(p);
        }

        p.reset_color();
        for (auto &i : m.items)
        {
            i->update_status(p);
        }
        show_color(p);

        for (auto &i : m.items)
        {
            print_to_client(i->display_text(p));
        }

        print_to_client(m.display_cell(p));

        /*if (m.status & exit_found)
        {
            m.set_level(p);
        }*/
    }

    return EXIT_SUCCESS;
}

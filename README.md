# esp8266_maze

## quick start
You can quickly compile and test with linux using the command: ```cd esp8266_maze && g++ linux_maze.cpp src/*.cpp -Isrc -o maze && ./maze```

If you want to see the part of maze loaded into memory you can use (you will have to install libSDL2 first): ```cd esp8266_maze/displayer && gcc displayer.c -lSDL2 -o display && g++ ../linux_maze.cpp ../src/*.cpp -I../src -o maze && ./maze out | ./display```

This will create a screenshot.bmp inside esp8266_maze/displayer as ![this](https://user-images.githubusercontent.com/65669679/115507831-e82f7200-a27c-11eb-9026-265e40b75a56.png)

To burn to an esp8266, you should be able to use the arduino ide and the .ino file in the src folder.

## some details
* This project is designed to run on an esp8266 and runs using https://github.com/akouz/coos for scheduling. You need to add it to your arduino ide library folder :)
* The Maze Generator is a modified Sidewinder algorithm (https://weblog.jamisbuck.org/2011/2/3/maze-generation-sidewinder-algorithm). It basically works like Sidewinder but it adds additional random southward paths in each run set. This change allows for loops in the maze (multiple paths from cell to cell) and helps a lot when you need to "go north".
* The map is almost infinite and only a small part is loaded into the esp8266's small memory (the default is 100x100). So, we use the seed of the RNG to have a "predictable randomization": each row initializes the seed to the row number and then we have to "just" make sure that we always use the same number of rand() calls  to randomly dig the labyrinth.
* All characters or objects must implement their own class derived from the Item class. The main procedure will use the specific method (if provided) for each item when the player arrives in a new cell.

# esp8266_maze

## quick start
To burn to an esp8266, you should be able to use the arduino ide and the .ino file in the src folder. Then you can connect with telnet (with this run I have a lot of chance finding the compass on the first move!):
```
$ telnet 192.168.XXX
Trying 192.168.XXX...
Connected to 192.168.XXX.
Escape character is '^]'.

v0.3 Apr 26 2021
 ______  __, _ __    _ _ _            
(  /    (   ( /  )  ( / ) )           
  /--    `.  /--'    / / / __,  __, _ 
(/____/(___)/       / / (_(_/(_/_/_(/_
                               (/     
                                      
Welcome stranger, this labyrinth is now your final home
You are right to be afraid (and you could press q for asking the gods help and get out of there)
If you have enough bravery, you can move with : 
   i         8
 j k l     4 2 6
You may meet people or objects that you may find useful for finding the exit ...
Pos: 1000,1933 Possible paths:  S  
Pos: 1001,1933 Possible paths: N EW
Pos: 1001,1932 Possible paths: N EW
Pos: 1001,1931 Possible paths: N E 
Pos: 1000,1931 Possible paths: NS  
Pos: 999,1931 Possible paths:  SEW
Pos: 999,1932 Possible paths:    W
Pos: 999,1931 Possible paths:  SEW
Pos: 999,1930 Possible paths:   E 
Pos: 999,1931 Possible paths:  SEW
Pos: 1000,1931 Possible paths: NS  
[...]
```

## linux debug version
You also can quickly compile and test with linux using the command: ```cd esp8266_maze && g++ linux_maze.cpp src/*.cpp -Isrc -o maze && ./maze```

If you want to see the part of maze loaded into memory you can use (you will have to install libSDL2 first): ```cd esp8266_maze/displayer && gcc displayer.c -lSDL2 -o display && g++ ../linux_maze.cpp ../src/*.cpp -I../src -o maze && ./maze out | ./display```

This will create a screenshot.bmp inside esp8266_maze/displayer as ![screenshot](https://user-images.githubusercontent.com/65669679/115605475-d84a7900-a2e2-11eb-811f-e12ee4cb8b36.png)

If you want to check a path search algorithm, A* can be run with (you will have to install libSDL2 first): ```cd esp8266_maze/displayer && gcc resolver.c list.c bin_heap.c -lSDL2 -o resolve && g++ ../linux_maze.cpp ../src/*.cpp -I../src -o maze && ./maze out | ./resolve```

## some details
* This project is designed to run on an esp8266 and runs using https://github.com/akouz/a_coos for scheduling. You need to add it to your arduino ide library folder :)
* You will also need to add the Arduino_AVR STL library for the classic C++ structures, Wifimanager for the wifi portal, and ESPTelnet
* The Maze Generator is a modified Sidewinder algorithm (https://weblog.jamisbuck.org/2011/2/3/maze-generation-sidewinder-algorithm). It basically works like Sidewinder but it adds additional random southward paths in each run set. This change allows for loops in the maze (multiple paths from cell to cell) and helps a lot when you need to "go north".
* The map is almost infinite and only a small part is loaded into the esp8266's small memory (the default is 100x100). So, we use the seed of the RNG to have a "predictable randomization": each row initializes the seed to the row number and then we have to "just" make sure that we always use the same number of rand() calls  to randomly dig the labyrinth.
* All characters or objects must implement their own class derived from the Item class. The main procedure will use the specific method (if provided) for each item when the player arrives in a new cell.

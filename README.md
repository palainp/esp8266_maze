# esp8266_maze

## quick start
You can quickly compile and test with linux using the command: ```cd esp8266_maze && g++ linux_maze.cpp src/*.cpp -Isrc -o maze && ./maze```

If you want to see the part of maze loaded into memory you can use (you will have to install libSDL2 first): ```cd esp8266_maze/displayer && gcc displayer.c -lSDL2 -o display && g++ ../linux_maze.cpp ../src/*.cpp -I../src -o maze && ./maze out | ./display```

This will create a screenshot.bmp inside esp8266_maze/displayer as ![this](https://user-images.githubusercontent.com/65669679/115507831-e82f7200-a27c-11eb-9026-265e40b75a56.png)

To burn to an esp8266, you should be able to use the arduino ide and the .ino file in the src folder. Then you can connect with telnet (with this run I have a lot of chance finding the compass on the first move!):
```
$ telnet 192.168.XXXX
Trying 192.168.XXXX...
Connected to 192.168.XXXX.
Escape character is '^]'.
v0.2 Apr 21 2021
 ______  __, _ __    _ _ _            
(  /    (   ( /  )  ( / ) )           
  /--    `.  /--'    / / / __,  __, _ 
(/____/(___)/       / / (_(_/(_/_/_(/_
                               (/     
                                      
[...]
Pos: 1000,1933 Possible paths:  S  
You just found the compass !
Pos: 1001,1933 Possible paths: NSEW Exit: 1000,1943
Pos: 1001,1932 Possible paths: NSEW Exit: 1000,1943
Pos: 1001,1931 Possible paths: N E  Exit: 1000,1943
Pos: 1000,1931 Possible paths: NS   Exit: 1000,1943
Pos: 999,1931 Possible paths:  SEW Exit: 1000,1943
Pos: 999,1932 Possible paths: N  W Exit: 1000,1943
[...]
``̀

## some details
* This project is designed to run on an esp8266 and runs using https://github.com/akouz/coos for scheduling. You need to add it to your arduino ide library folder :)
* The Maze Generator is a modified Sidewinder algorithm (https://weblog.jamisbuck.org/2011/2/3/maze-generation-sidewinder-algorithm). It basically works like Sidewinder but it adds additional random southward paths in each run set. This change allows for loops in the maze (multiple paths from cell to cell) and helps a lot when you need to "go north".
* The map is almost infinite and only a small part is loaded into the esp8266's small memory (the default is 100x100). So, we use the seed of the RNG to have a "predictable randomization": each row initializes the seed to the row number and then we have to "just" make sure that we always use the same number of rand() calls  to randomly dig the labyrinth.
* All characters or objects must implement their own class derived from the Item class. The main procedure will use the specific method (if provided) for each item when the player arrives in a new cell.

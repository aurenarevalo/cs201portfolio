# cs201portfolio
cs201 portfolio, planning an "infection"-style game

# Recommended settings

It is recommended that you keep the terminal at the default size,

    Default size = 24 rows, 80 cols

Otherwise, you may experience unexpected behavior.

# Virtual Machine users

Ensure that your Virtual Machine window size is large enough and you're running terminal _separately_. 

**_THIS GAME HAS BEEN TESTED ON A DUAL BOOT OF UBUNTU LTS 16.04 AND 18.04_** 

# NOTE
Any input greater than 2,147,483,647 (2^31 -1) will always generate an error.

Given you have enough memory, the game should be able to run fine at a large size, but the user experience will definitely be less enjoyable the larger the board size gets due to load times and management of many boards.

# UNIMPLEMENTED CODE

There is code that is unimplemented.

There is code that is uncommented due to time constraints.

Player vs Player is unimplemented. Only AI works.


# HOW TO PLAY

After the splash screen displays, you can enter your game size (how many rows and columns of boards).

Each board will contain 4-10 nodes, where 2 or 4 are equal quantities of enemy and player nodes.

You can disable to splash screen by running the game as: `./main nosplash`. Anything else will result in the splash screen.


- ![#1589F0](https://placehold.it/15/00ffff/000000?text=+) `#00ffff` - CYAN = Player node
- ![#c5f015](https://placehold.it/15/ffff00/000000?text=+) `#ffff00` - YELLOW = Enemy node
- ![#f03c15](https://placehold.it/15/ff00ff/000000?text=+) `#ff00ff` - MAGENTA = Neutral node

**SINGLE LEFT CLICK** on a player-controlled node will visually change (white "outline") to show it is selected. Another **LEFT CLICK** on it will _DESELECT_ it. 

While selected, you can choose any node on the board to **ATTACK**, sending **HALF** of the nodes current units to the target. 

You can **send units to other nodes under your control**, which will add what you send to it.

The **_MAXIMUM_** units on any node is **100**, so think carefully about sending to allied nodes!


**WIN CONDITIONS**:

* No nodes of the opposing color exist
* Have the most nodes when all neutral nodes are gone


**_MIDDLE CLICK_** will pass your turn (mouse recommended).

Press `q` to quit at any time

**To navigate the different windows, click anywhere while no node is selected:**

* DOUBLE LEFT CLICK --> x+1;

* TRIPLE LEFT CLICK --> y+1;

* DOUBLE RIGHT CLICK --> x-1;

* TRIPLE RIGHT CLICK --> y-1;

~~There's a bug still I haven't had time to fix due to changes in how the game works 
to allow for more efficient generation of variable board sizes.~~

All known bugs were fixed.

# FILE EXPLANATIONS

* **main.c**
	* Main file contains interface initializations, splash screen, and the call to the game loop

* **scene.c** AND **scene.h**
	* File built to make managing windows and menus much easier than the standard shipping of ncurses.
	- A scene consists of an array of menus and panels

* **gamemanager.c** AND **gamemanager.h**
  * Manages game logic and game loop, generation of grid, and any game-oriented interface actions.
* **button.c** AND **button.h**
  * Small file for an addition to ncurses: BUTTONS!
  * Buttons are simply derived windows of a parent window that are placed relative to the parent window position. Includes a function for click detection.
* **infection_ds.c** AND **infection_ds.h**
  * Contains base data structures and functions for the structures used elsewhere in files.
* **Keycodes.h**
  * deprecated; enum states relating to key codes for readability of code in menu management.
* **utility.c** AND **utility.h**
  * "utility" functions that didn't necessarily have a place elsewhere. Some may  not be used.
* **gamestates.h** 
  * contains an enum typedef that defines gamestates. Helps readability in game loop.
  
  
  # ANOTHER NOTE
  
  The game generates all positions and controls of nodes after entering dimensions, and only draws/creates windows when requested based on window traversal (it's only one window that holds the buttons).

I tried to make all the code as legible as possible with good nomenclature.

Apologies if the window traversal is wonky. Again, a mouse is recommended, but not required.
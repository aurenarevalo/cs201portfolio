# cs201portfolio
cs201 portfolio, planning an "infection"-style game

# Recommended settings

It is recommended that you keep the terminal at the default size,
	-Default size = 24 rows, 80 cols
Otherwise, you may experience unexpected behavior.


#NOTE
Any input greater than 2,147,483,647 (2^31 -1) will always generate an error.
Given you have enough memory allocated to the heap, the game should be able
to run fine at a large size, but the user experience will definitely be 
less enjoyable the larger the board size gets.

#UNIMPLEMENTED CODE

There is code that is unimplemented.
There is code that is uncommented due to time constraints.
Player vs Player is unimplemented. Only AI works.


#HOW TO PLAY

BLUE NODES = player nodes
YELLOW NODES = enemy nodes
MAGENTA NODES = Neutral nodes

Your goal is to overtake the enemy. You do not have to take over every node, just every enemy node.

MIDDLE CLICK will pass your turn (mouse recommended).
press 'q' to quit at any time

To navigate the different windows:

DOUBLE LEFT CLICK --> x+1;
TRIPLE LEFT CLICK --> y+1;
DOUBLE RIGHT CLICK --> x-1;
TRIPLE RIGHT CLICK --> y-1;

There's a bug still I haven't had time to fix due to changes in how the game works 
to allow for more efficient generation of variable board sizes.
Pong Wars
=========

Pong Wars is Pong with a spicy twist. You are given two special abilities,
Time Wrap and Adrenaline Mode. Use these two powerful abilities at the right
time and you will find yourself in control of the field.

I created Pong Wars to learn C++ and SDL back in 2006. I have only tried
compiling it on Linux.

Installing
---------

$ make
$ ./pongwars

Objective
---------

Defeat the other person. There are no game-based goals as of now.

Options
---------
	
	vs Human
		- Player 1 vs Player 2, controls are below

	vs Computer
		- Currently, there is only one difficulty level: normal.

Controls
---------

	General:
		Esc - Quit.
		
	Player 1:
		W - up
		S - down
		D - Time Wrap
		A - Adrenaline Mode

	Player 2:
		Arrow up - up
		Arrow down - down
		Arrow right - Time Wrap
		Arrow left - Adrenaline Mode

Specials
---------

  Time Wrap - Either reduces the speed of the ball if it is coming towards
        you or increases the speed of the ball if it is going away from you.
        This special may reverse the direction of the ball.

        Special 1 can only be used once per round and are recharged when you
        score on your opponent. Everytime someone scores, a new round
        begins.

   Adrenaline Mode - In Adrenaline Mode, the player that makes the first
        goal gains 2 points, while the defender loses 1 point.

        Adrenaline Mode is activated once a player earns enough points.
        After activation, the player may activate Adrenaline Mode at any
        time.


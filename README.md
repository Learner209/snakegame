# Snakegame

## Features

***
*	Provides more APIs to manipulate the snake and to modify relevant features.
*	Actualizes the Double mode where two opponents maneuver their own snakes in their own windows\.
*	Furnishes an API for adjusting the countdown under the limits of which players are able to compete\.
*	Implements the machine snake whose smartness is dynamically modified by the moving speed and the fixed-in difficulty\.
*	Copious terrains implemented to make the movement of the snake colorful\.
***

## Details

***

### Operations

*	*ENTER*: choose the **HIGHLIGTED** item\.
*	*BACKSPACE or WHITESPACE*: back to the parent menu\.
*	*MOUSE*: use mouse to click the item in a menu or click the *CLOSE* button to close the current window\.
*	*WSAD or Up/Down/Left/Right*: When in *SOLO* or the *"Man to Computer"* mode under the *Double* mode, *WSAD* or *Up/DOwn/Left/Right* can all be used to maneuver the snake. When in the *"Man to Man* under the *Double* mode, *WSAD* can be used to maneuver the snake in the left window, *Up/DOwn/Left/Right* can be used to maneuver the snake in the right window\.
### Mode

#### Solo

***

*	*Difficulty*: the emblem of the velocity of the snake. The velocity of the snake skyrockets in exponential frame as the difficulty rises up. The underlying mechanism denotes that the velocity of the snake is 1 / \( 0\.75 ^ Difficulty\) units per milliesecond.
*	*Has dynamic difficulty*: the API provided for users to control whether the velocity will be dynamically modified when our snake is on its wonderful journey. When it is on, the velocity of the speed will ascend (the Difficulty will rise by one) whenever the player gain five more points.
*	*Has walls*: the API furnished for users to decide whether the field has walls along the process. 

#### Double

***

*	*Speed*: Velocity of the snake. The velocity of the snake skyrockets in exponential frame as the difficulty rises up. The underlying mechanism denotes that the velocity of the snake is 1 / \( 0\.8 ^ Difficulty\) units per milliesecond \(The base is modified form 0.75 to 0.8 as the player enjoys the game in a smaller frame of window\).
*	*Countdown*: The countdown under which two players are to compete. When altering the countdown, the cursor will be immediately trailing the user's input. The Backspace key will enable the user to revert to the previous digit. If feeling uncomfortable with the current change, feel free to touch the first digit to revise the countdown again. \(Notice: A warning window will pop up when you inputs a number invalid\. A valid countdown is deemed to be a time denoting in the format of 'minutes:seconds' anywhere between 0:00 to 9:59\.)
*	*Has walls*: it works the same with the one in the Solo mode.
*	*Man to ?*: The opponent type. You will be confronting you peer under "Man to Man". However, you will be confronting a machine snake whose wisdom and smartness tops the pinnacle of human's intelligence.
*	*Difficulty*: this item will only show up if you are under "Man to Computer Mode". Categorized into *"Easy"*, *"Normal"*, *"Medium"*, *"Hard"*, the difficulty is the epitome of the smartness of the machine snake. Tricky and prudent as the machine snake is, this Difficulty may still save you from being smashed by the computer. The underlying mechanism is to use the theory of probability and four indexs to compose the overall intelligence level of the machine snake. \(Mistackle: the indication of the snake's probability of missing the food. Strike: the embodiment of the snaks's capability of hitting the food in short range. MoveBack: the abstraction of the snake's ability of the sharpness of sensing the food. Collision: the conceptualization of the snake's ability to avoid the collision. The machine snake operates on these four indexes which are dynamically altered in accordance with the Difficulty the uses has chosen and the velocity of the two snakes.\)\.

#### Survival Mode
***

#### Crucial Mode
***

### Terrains
*	*Plain*: boundless and flat plain for our snake to embark on its adventure\. No
 effects\.
*	*Water*: Shiny waves and broad blue vista\. Unfortunately, our snake hasn't been managed to learn to swim since its childhood. Once the snake's head is in the water, it is drowning desperately and the progress bar in the upper window will be indicating the drowning countdown. And if it has been drowning for more than five seconds, our snake will be gone forever...
*	*Mountain*: Steep hills and hard rocks/. Once the head hits the mountain. Our wretched snake will have to say farewell to its wonderful journey.
*	*Forest*: Dense trees and large marshes. Once the snake's head writhes into this dense forest, the velocity will decrease by half\.
	
### Barriers yet encountered
*	*Machine Snake*: Cannot come up with a perfect algorithm for the machine snake to avoid trapping itself into a pitfall under the traditinal frame of "if else" and "while" clauses\. The fundamental checks other than hitting itself, wall and the mountain are the circuit check in which the snake is required to take measures before forming a circuit which could be the premonition of a crash\.
*	*Multi-thread*: When in the *Double* mode, two players should be playing in their seperate boundaries, and this can be preferably implemented using std::thread. Executing two threads dictated by two lambda functions, each of which is used to maneuver its own snake and render its relevant information and even to manipulate the snake at different rates\. But because the ncurses environment operates on static variables to execute the render action, multi-threads can only operate stably when only one channel under which occasion other threads are to be independent of the GUI library and finish the underlying computing procedures\.  

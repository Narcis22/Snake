# Snake

Introduction to Robotics Homework 5, 3rd year at the Faculty of Matematics and Computer Science, section Computer Science at the University of Bucharest. This is composed by the classic `Snake` game recreated from scratch by me (Narcis Necula). Here you will find a detailed description of the game logic and parts as well as photos and a video playing the game.

### 🐍 Snake (Narcis's version) 🍎

>The game is composed of two main parts:
  - The menu (which is presented on a lcd display -https://docs.arduino.cc/learn/electronics/lcd-displays-) 
  
  - The Game (which is the presented on an 8x8 matrix display)
 
> The menu has all the functionality like: brightness controls for the lcd and the 8x8 matrix display,
has sound off/on funcntions, difficulty control, has the option to reset the Leaderboard scores, he can insert the 
name under which he can play the game has information about the game and the owner and `How To Play` guide.

> In Game the Display shows "stats" like: Name of the player (if he hasn't inserted a name in the settings menu 
then he will see `ANONI`), level(which will be described in detail later), lives left and acumulated score.

> After the game ends we have a message that tells us the game has ended, then if we had didn't have a top 5  score
we will see a message that informs us of that. If the score we obtained has a place on the leaderboard then we will 
anounce that, and if a name wasn't inserted in the settings menu then one will be required now to be placed on the 
leaderboard , and after inserting a name you will have the option to save it in the settings as well.

> The game has at the base the classic game `Snake`. My spin on it is as follows:
There are 6 levels that a player can pass trough. Levels one and two yeald 10 points per apple eaten , level three 
and four yeald 30 points per apple eaten and levels 4 and up yeald 50 points per apple eaten.
With each level the difficulty of the game increases as follows: On each level the speed of the snake increases, 
and on the last level we have disabled the wall warping function (passwing trough walls and coming out on the opposide
side).

> Sounds will be played when navigating trough the menu and when the snake eats apples as well as when the
sake dies. These sounds will be disabled if the user chooses the mute function in the settings menu.
Also the 8x8 display shows suggestive images for the menu that the user hovers above (has the arrow pointed at it)
like: a play button image on the `Start Game` menu, a trophy on the `Leaderboard` and so on (you get the jist).

> The Project also contains phisical dials (potentiometers) for the LCD contrast function and the Buzzer volume 
control.

### Used Hardware🕹️


 - potentiometers - img
 - Joystick - img
 - buzzer - img
 - lcd - link to page
 - 8x8 matrix display - lcd
 - processor - MAX7219 LED Driver
 - arduino board - link
 
 
### 🧾 Rules of the game from the Introduction to Robotics class that have to be met:

 - Minimal components: an LCD, a joystick, a buzzer and the led matrix.
 
 - You must add basic sounds to the game (when ”eating” food, when dying, when finishing the level etc). Extra: add theme songs.
 
 - It must be intuitive and fun to play.
 
 - It must make sense in the current setup. Study the idea of a panning camera - aka the 8x8 led doesn’t need to be the entire map. It can only be the current field of view of the player.
 
 - The levels must progress dynamically. Aka the level difficulty, score and other properties should be a function of the level number or time. However, the challenge here is for it to grow in a playable way - not too easy for a long time so it becomes boring, not too hard too fast so it’s not attractive. Also, it’s nice to have an ending, a final level, a boss etc. It shouldn’t necessarily go on forever (but that’s fine, though).

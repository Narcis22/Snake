# Snake

Introduction to Robotics Homework 5, 3rd year at the Faculty of Matematics and Computer Science, section Computer Science at the University of Bucharest. This is composed by the classic `Snake` game recreated from scratch by me (Narcis Necula). Here you will find a detailed description of the game logic and parts as well as photos and a video playing the game.

### 🐍 Snake (Narcis's version) 🍎

>The game is composed of two main parts:
  - The menu (which is presented on a lcd display) 
  
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
![buzzer](https://user-images.githubusercontent.com/62501946/207729564-6614f6fb-4cfc-4f41-8e14-e4bdb768c869.png)

> Sounds will be played when navigating trough the menu and when the snake eats apples as well as when the
sake dies. These sounds will be disabled if the user chooses the mute function in the settings menu.
Also the 8x8 display shows suggestive images for the menu that the user hovers above (has the arrow pointed at it)
like: a play button image on the `Start Game` menu, a trophy on the `Leaderboard` and so on (you get the jist).

> The Project also contains phisical dials (potentiometers) for the LCD contrast function and the Buzzer volume 
control.

### ![motherboard](https://user-images.githubusercontent.com/62501946/207729702-9d0b2461-6e45-44bb-822e-b9c9e249d3ad.png) ![buzzer](https://user-images.githubusercontent.com/62501946/207729581-dfef9b80-1f5d-4c7e-b015-9d15bbeb6033.png) Used Hardware🕹️![lcd](https://user-images.githubusercontent.com/62501946/207729283-d9806b57-faa1-4b8f-b62e-b0122991240a.png)


 - [Potentiometers](https://arduinogetstarted.com/tutorials/arduino-potentiometer) - Two of them for phisically controling the lcd contrast and the volume for the buzzer.
 - [Joystick](https://components101.com/modules/joystick-module)
 - [Buzzer](https://components101.com/misc/buzzer-pinout-working-datasheet)
 - [LCD details](https://docs.arduino.cc/learn/electronics/lcd-displays)
 - [8x8 matrix display](https://create.arduino.cc/projecthub/SAnwandter1/programming-8x8-led-matrix-23475a)
 - [MAX7219 - Driver for LCD](https://www.analog.com/media/en/technical-documentation/data-sheets/max7219-max7221.pdf)
 - [Arduino UNO R3 board](https://docs.arduino.cc/static/9d6ed041fec691039663ae42f50fabcc/A000066-datasheet.pdf)
 
 
### 🧾 Rules of the game from the Introduction to Robotics class that have to be met:

 - Minimal components: an LCD, a joystick, a buzzer and the led matrix.
 
 - You must add basic sounds to the game (when ”eating” food, when dying, when finishing the level etc). Extra: add theme songs.
 
 - It must be intuitive and fun to play.
 
 - It must make sense in the current setup. Study the idea of a panning camera - aka the 8x8 led doesn’t need to be the entire map. It can only be the current field of view of the player.
 
 - The levels must progress dynamically. Aka the level difficulty, score and other properties should be a function of the level number or time. However, the challenge here is for it to grow in a playable way - not too easy for a long time so it becomes boring, not too hard too fast so it’s not attractive. Also, it’s nice to have an ending, a final level, a boss etc. It shouldn’t necessarily go on forever (but that’s fine, though).

<details> <summary><i><b>Setup photos</b></i> </summary>
</br></br>

![20221214_232211342_iOS](https://user-images.githubusercontent.com/62501946/207737898-279f9f17-d48d-4e5e-997a-bc634b0c9fa4.png)

![20221214_232251512_iOS](https://user-images.githubusercontent.com/62501946/207737905-ff8598e9-ded5-449e-9e41-ce6b6e913294.png)

![20221214_232304154_iOS](https://user-images.githubusercontent.com/62501946/207737920-27c22a24-fd3b-4fe1-bc30-a1c77739129c.png)

![20221214_232223030_iOS](https://user-images.githubusercontent.com/62501946/207737909-bb1bd7fe-0374-4200-8e98-baeb6a893652.png)

![20221214_232217539_iOS](https://user-images.githubusercontent.com/62501946/207737916-c384b0bb-9450-4035-a0a8-be6f96a91ca9.png)

</details>

[Code](https://github.com/Narcis22/Snake/blob/main/Snake/Snake.ino)

![youtube](https://user-images.githubusercontent.com/62501946/207731145-e389b0ed-5e28-46d1-b37d-ae46b870f8c5.png) <i><b>Video with walktrough and playtrough: </b></i>
[Go to youtube to see video.]()

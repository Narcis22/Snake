#include <LiquidCrystal.h>
#include "LedControl.h"
#include <EEPROM.h>
#include "pitches.h"

const byte dinPin = 12;
const byte clockPin = 11;
const byte loadPin = 10;
const byte matrixSize = 8;

const byte RS = 9;
const byte enable = 8;
const byte d4 = 7;
const byte d5 = 3;
const byte d6 = 5;
const byte d7 = 4;
const byte lcdBrightnessControlPin = 6;

LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);
LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);

// joystick setup
const byte pinSW = 2;
const byte pinX = A0;
const byte pinY = A1;

byte swState = HIGH;
byte lastSwState = HIGH;

int xValue = 0;
int yValue = 0;

bool xJoystickState = false;
bool yJoystickState = false;
const byte minThreshold = 200;
const int maxThreshold = 800;

byte reading = 1;
unsigned long lastDebounceTime = 0;
const byte debounceDelay = 50;
unsigned long startGreetingTime = 0;
const unsigned int greetingDuration = 5000;
const unsigned int gameOverDuration = 5000;
unsigned long startGameOverSequence = 0;

bool hasChosenSaveOption = false;

byte indexText = 1;

byte menuState;
byte mainMenuView;
byte infoMenuView;
byte settingsMenuView;
byte htpMenuView;
byte leaderboardView;

bool saveName;
bool hasNameSet = false;
String playerName = "AAAAA";

byte yJoystickPosition = 0;
byte difficulty = 0;
byte sesionDifficulty = 0;
bool activateSound;
bool soundHasPlayed = false;

byte lcdBrightness;
byte matrixBrightness;

byte brightnessItem; // matrix or lcd

byte noExistentHighscores;
String leaderboardNames[5] = {};
long leaderboardScores[5] = {};

bool scoreMarked = false;
byte insertPosition;

bool playing;

struct {
  byte x, y;
} snakeBody[58], applePosition;

byte snakeLength = 2; 
byte direction = 0;
byte indexMove = 0;

unsigned long lastMovement = 0;
unsigned int speed[7] = {450, 400, 350, 300, 250, 200, 150};

bool hasEatenApple = false;

byte matrixLayout[8][8] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};

long score;
byte livesLeft;

// blinking interval
const int blinkTimeOn = 400;
unsigned long lastBlink = 0;

byte buzzer = A2;

// Custom characters
byte arrowDown[8] = {
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
  0b10001,
	0b01010,
	0b00100
};

byte speaker[8] = {
  0b00000,
  0b00010,
  0b00110,
  0b01110,
  0b01110,
  0b00110,
  0b00010,
  0b00000
};

byte soundOff[8] = {
  0b00000,
  0b00000,
  0b10010,
  0b01100,
  0b01100,
  0b10010,
  0b00000,
  0b00000
};

byte soundOn[8] = {
  0b11000,
  0b00100,
  0b10010,
  0b01010,
  0b01010,
  0b10010,
  0b00100,
  0b11000
};

byte brightnessBar[8] = {
  0b01110,
  0b01110,
  0b01110,
  0b01110,
  0b01110,
  0b01110,
  0b01110,
  0b01110
};

byte heart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};

// Matrix custom images
const uint8_t DisplayImages[][8] = {
{
  0b11111111, // 0 spiral
  0b10000001,
  0b10111101,
  0b10100101,
  0b10101101,
  0b10100001,
  0b10111111,
  0b10000000
},{
  0b10100101, // 1 Dead face
  0b01000010,
  0b10100101,
  0b00000000,
  0b01111110,
  0b00001100,
  0b00001100,
  0b00000000
},{
  0b00100000, // 2 Play
  0b00110000,
  0b00111000,
  0b00111100,
  0b00111100,
  0b00111000,
  0b00110000,
  0b00100000
},{
  0b01111110, // 3 Trophy
  0b00111100,
  0b11111111,
  0b10111101,
  0b11111111,
  0b00111100,
  0b00011000,
  0b00111100
},{
  0b00000100, // 4 Speaker lound
  0b00100010,
  0b01101010,
  0b11101010,
  0b11101010,
  0b01101010,
  0b00100010,
  0b00000100
},{
  0b00011000, // 5 info
  0b00011000,
  0b00000000,
  0b00111000,
  0b00011000,
  0b00011000,
  0b00011000,
  0b00111100
},{
  0b00010000, // 6 Brightness
  0b01010010,
  0b00111100,
  0b00111111,
  0b11111100,
  0b00111100,
  0b01001010,
  0b00001000
},{
  0b00000000, // 7 speaker mute
  0b00100000,
  0b01101001,
  0b11100110,
  0b11100110,
  0b01101001,
  0b00100000,
  0b00000000
}};

int whatToDisplay = 0;
int whatIsDisplayed = 0;

void setup() {
  
  Serial.begin(9600);
  
  lcd.begin(16, 2);
  pinMode(pinSW, INPUT_PULLUP);
  
  pinMode(lcdBrightnessControlPin, OUTPUT);

  // initializeEEPROM(); // run only once to insert default values in eeprom / reset all values in eeprom
  setDataFromEEPROM();
  getLeaderboardFromEEPROM();
  
  analogWrite(lcdBrightnessControlPin, lcdBrightness);
  lc.shutdown(0, false);
  lc.setIntensity(0, matrixBrightness);
  
  printImageOnMatrix(DisplayImages[0]);

  startGreetingTime = millis();
  menuState = 0;
  mainMenuView = 0;

  lcd.createChar(7, arrowDown);
}

void loop() {
  if (whatToDisplay != whatIsDisplayed) {
    printImageOnMatrix(DisplayImages[whatToDisplay]);
    whatIsDisplayed = whatToDisplay;
  }

  xValue = analogRead(pinX);
  yValue = analogRead(pinY);

  reading = digitalRead(pinSW);
  if (reading != lastSwState) {
    lastDebounceTime = millis();
  }

  if (millis() - startGreetingTime < greetingDuration) {
    lcd.setCursor(0, 0);
    lcd.print("    Snake");

    lcd.setCursor(0, 1);
    lcd.print("       by Narcis");
  }
  else {
    if (millis() - lastDebounceTime > debounceDelay) {
      if (reading != swState) {    
        swState = reading;
      }
    }
    lastSwState = reading;

    if (menuState == 0) {
      
      delay(30);
      
      switch (mainMenuView) {
        case 0:
          whatToDisplay = 2;
      
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("> Start Game");

          lcd.setCursor(1, 1);
          lcd.print("Leaderboard");

          lcd.setCursor(15, 1);
          lcd.write((byte)7);

          delay(50);

          if (swState == LOW){
            swState = HIGH;
            lastSwState = HIGH;            
            playing = true;

            snakeBody[0].x = 0;
            snakeBody[0].y = 0;
            snakeBody[1].x = 0;
            snakeBody[1].y = 1;
            snakeLength = 2; 
            direction = 0;
            sesionDifficulty = difficulty;

            for (int i = 0; i < matrixSize; ++i) {
              for (int j = 0; j < matrixSize; ++j) {
                lc.setLed(0, i, j, false); // turns off LED at col, row
                matrixLayout[i][j] = 0;
              }
            }

            for (int i = 0; i < snakeLength; ++i) {
              lc.setLed(0, snakeBody[i].x, snakeBody[i].y, true);
              matrixLayout[snakeBody[i].x][snakeBody[i].y] = 1;
            }

            score = 0;
            livesLeft = 3;
            soundHasPlayed = false;

            lcd.createChar(0, heart);
            lcd.clear();
            lcd.setCursor(1, 0);
            lcd.print("Start game");
            
            delay(50);
            menuState = 1;
          }

          mainMenuView = setMenuState(mainMenuView, 1, mainMenuView); 
          break;
        
        case 1: // Leaderboard
          whatToDisplay = 3;

          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Start Game");
          lcd.setCursor(0, 1);
          lcd.print("> Leaderboard");
          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW){
            leaderboardView = 0;
            menuState = 2;
            swState = HIGH;
            lastSwState = HIGH;
          }

          mainMenuView = setMenuState(mainMenuView, 2, 0);
          break;

        case 2: // Settings
          whatToDisplay = 0;

          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Leaderboard");
          lcd.setCursor(0, 1);
          lcd.print("> Settings");

          lcd.setCursor(15, 0);
          lcd.print("^");

          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW){
            settingsMenuView = 0;
            menuState = 3;
            swState = HIGH;
            lastSwState = HIGH;
          }

          mainMenuView = setMenuState(mainMenuView, 4, 3);
          break;

        case 3: // Leaderboard
          whatToDisplay = 3;

          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("> Leaderboard");

          lcd.setCursor(1, 1);
          lcd.print("Settings");

          lcd.setCursor(15, 0);
          lcd.print("^");

          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW){
            leaderboardView = 0;
            menuState = 2;
            swState = HIGH;
            lastSwState = HIGH;
          }

          mainMenuView = setMenuState(mainMenuView, 2, 0);
          break;

        case 4: // About
        
          whatToDisplay = 5;

          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Settings");

          lcd.setCursor(0, 1);
          lcd.print("> About");

          lcd.setCursor(15, 0);
          lcd.print("^");

          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW){
            infoMenuView = 0;
            menuState = 4;
            swState = HIGH;
            lastSwState = HIGH;
          }

          mainMenuView = setMenuState(mainMenuView, 6, 5);
          break;

        case 5: // Settings
          whatToDisplay = 0;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("> Settings");

          lcd.setCursor(1, 1);
          lcd.print("About");

          lcd.setCursor(15, 0);
          lcd.print("^");
          
          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW){
            settingsMenuView = 0;
            menuState = 3;
            swState = HIGH;
            lastSwState = HIGH;
          }

          mainMenuView = setMenuState(mainMenuView, 4, 3);
          break;
        
        case 6: // How To Play
          whatToDisplay = 5;
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("About");

          lcd.setCursor(0, 1);
          lcd.print("> How to play");

          lcd.setCursor(15, 0);
          lcd.print("^");
          delay(50);

          if (swState == LOW){
            htpMenuView = 0;
            menuState = 5;
            swState = HIGH;
            lastSwState = HIGH;
          }

          mainMenuView = setMenuState(mainMenuView, mainMenuView, 7);
          break;
        
        case 7: // About
          whatToDisplay = 5;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("> About");

          lcd.setCursor(1, 1);
          lcd.print("How to play");

          lcd.setCursor(15, 0);
          lcd.print("^");
          delay(50);

          if (swState == LOW){
            infoMenuView = 0;
            menuState = 4;
            swState = HIGH;
            lastSwState = HIGH;
          }

          mainMenuView = setMenuState(mainMenuView, 6, 5);
          break;
      } 
    } else if (menuState == 1) {
      if (playing) {

        applesCreate();

        displayInGameStats();

        changeDirection();
        
        if (millis() - lastMovement > speed[sesionDifficulty]) {
          snakeMovement();
          lastMovement = millis();
        }

        if (!playing) {
          if (activateSound && !soundHasPlayed) {
            soundHasPlayed = true;
            gameOverSound();          
          }
          resetMatrix();
          startGameOverSequence = millis();
        }

      } else { 

        whatToDisplay = 1;        
                
        showGameEndedSequence();

        if (scoreMarked) {
          if (insertPosition >= 5) {
              // didn't make the cut
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("You didn't make");
              
              lcd.setCursor(0,1);
              lcd.print("the cut.");

            } else {
              if (insertPosition == noExistentHighscores) {
                // Leader board but not top 1(last in the leaderboard)
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("  You made it!");

                lcd.setCursor(0, 1);
                lcd.print("In the Top 5!");

              } else {
                if (insertPosition == 0) {
                  // Top 1
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("  You made it!");

                  lcd.setCursor(0, 1);
                  lcd.print(" Highest score!"); 

                } else {
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("  You made it!");

                  lcd.setCursor(0, 1);
                  lcd.print(" In the Top 5!"); 
                }
              }
            }

            if (swState == LOW) { 
              menuState = 0;
              swState = HIGH;
              lastSwState = HIGH;
              scoreMarked = false;
            }
          }
          delay(50);
        }
    
    } else if (menuState == 2) {  // view leaderboard Top 5
      if (noExistentHighscores == 0) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("  LEADERBOARD");

        lcd.setCursor(0, 1);
        lcd.print("Nobody Worthy!");
        delay(50);

      } else {
        switch (leaderboardView) {
          case 0:
            whatToDisplay = 3;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("  LEADERBOARD");

            lcd.setCursor(0, 1);
            lcd.print("1." + leaderboardNames[0]);

            lcd.setCursor(11, 1);
            lcd.print(leaderboardScores[0]);
            delay(50);

            leaderboardView = setMenuState(leaderboardView, 1, leaderboardView);
            break;
          
          case 1:
            whatToDisplay = 3;
            lcd.clear();

            lcd.setCursor(0, 0);
            lcd.print("1." + leaderboardNames[0]);

            lcd.setCursor(11, 0);
            lcd.print(leaderboardScores[0]);

            lcd.setCursor(0, 1);
            if (noExistentHighscores > 1) {
              lcd.print("2." + leaderboardNames[1]);

              lcd.setCursor(11, 1);
              lcd.print(leaderboardScores[1]);
            } else {
              lcd.print("2.");
            }
            delay(50);

            leaderboardView = setMenuState(leaderboardView, 2, 0);
            break;

          case 2:
            whatToDisplay = 3;
            lcd.clear();

            lcd.setCursor(0, 0);
            if (noExistentHighscores > 1) {
              lcd.print("2." + leaderboardNames[1]);

              lcd.setCursor(11, 0);
              lcd.print(leaderboardScores[1]);
            } else {
              lcd.print("2.");
            }

            lcd.setCursor(0, 1);
            if (noExistentHighscores > 2) {
              lcd.print("3." + leaderboardNames[2]);

              lcd.setCursor(11, 1);
              lcd.print(leaderboardScores[2]);
            } else {
              lcd.print("3.");
            }
            delay(50);

            leaderboardView = setMenuState(leaderboardView, 3, 1);
            break;

          case 3:
            whatToDisplay = 3;
            lcd.clear();

            lcd.setCursor(0, 0);
            if (noExistentHighscores > 2) {
              lcd.print("3." + leaderboardNames[2]);

              lcd.setCursor(11, 0);
              lcd.print(leaderboardScores[2]);

            } else {
              lcd.print("3.");
            }

            lcd.setCursor(0, 1);
            if (noExistentHighscores > 3) {
              lcd.print("4." + leaderboardNames[3]);

              lcd.setCursor(11, 1);
              lcd.print(leaderboardScores[3]);

            } else {
              lcd.print("4.");
            }
            delay(50);

            leaderboardView = setMenuState(leaderboardView, 4, 2);
            break;

          case 4:
            whatToDisplay = 3;
            lcd.clear();

            lcd.setCursor(0, 0);
            if (noExistentHighscores > 3) {
              lcd.print("4." + leaderboardNames[3]);

              lcd.setCursor(11, 0);
              lcd.print(leaderboardScores[3]);

            } else {
              lcd.print("4.");
            }

            lcd.setCursor(0, 1);
            if (noExistentHighscores > 4) {
              lcd.print("5." + leaderboardNames[4]);

              lcd.setCursor(11, 1);
              lcd.print(leaderboardScores[4]);

            } else {
              lcd.print("5.");
            }
            delay(50);

            leaderboardView = setMenuState(leaderboardView, 5, 3);
            break;

          case 5:
            whatToDisplay = 3;
            lcd.clear();

            lcd.setCursor(0, 0);
            if (noExistentHighscores > 4) {
              lcd.print("5." + leaderboardNames[4]);

              lcd.setCursor(11, 0);
              lcd.print(leaderboardScores[4]);

            } else {
              lcd.print("5.");
            }

            lcd.setCursor(0, 1);
            lcd.print("<- Go back");
            delay(50);
            leaderboardView = setMenuState(leaderboardView, leaderboardView, 4);
            
            break;
        }
      }

      if (swState == LOW) {
        menuState = 0;
        swState = HIGH;
        lastSwState = HIGH;
      }
    } else if (menuState == 3) {
      switch (settingsMenuView) {
        case 0:
          whatToDisplay = 0;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("> Enter name");
          
          lcd.setCursor(1, 1);
          lcd.print("Difficulty");

          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          settingsMenuView = setMenuState(settingsMenuView, 1, settingsMenuView);

          if (swState == LOW){
            yJoystickPosition = 0;
            settingsMenuView = 8;
            swState = HIGH;
            lastSwState = HIGH;
          }
          break;
        
        case 1:
          whatToDisplay = 0;
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Enter name");

          lcd.setCursor(0, 1);
          lcd.print("> Difficulty");

          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW){
            settingsMenuView = 9; 
            swState = HIGH;
            lastSwState = HIGH;
          }
          
          settingsMenuView = setMenuState(settingsMenuView, 2, 0);
          break;
        
        case 2:
          whatToDisplay = 6;
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Difficulty");
          
          lcd.setCursor(0, 1);
          lcd.print("> Brightness");
         
          lcd.setCursor(15, 0);
          lcd.print("^");

          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW) {
            brightnessItem = 0;
            settingsMenuView = 11; 
            swState = HIGH;
            lastSwState = HIGH;
          }
          
          settingsMenuView = setMenuState(settingsMenuView, 4, 3);
          break;
        
        case 3:
          whatToDisplay = 0;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("> Difficulty");

          lcd.setCursor(1, 1);
          lcd.print("Brightness");

          lcd.setCursor(15, 0);
          lcd.print("^");

          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW){
            settingsMenuView = 9; 
            swState = HIGH;
            lastSwState = HIGH;
          }
          
          settingsMenuView = setMenuState(settingsMenuView, 2, 0);
          break;

        case 4:
          whatToDisplay = 4;
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Brightness");

          lcd.setCursor(0, 1);
          lcd.print("> Sound");

          lcd.setCursor(15, 0);
          lcd.print("^");

          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW){
            settingsMenuView = 10; 
            swState = HIGH;
            lastSwState = HIGH;
          }
          
          settingsMenuView = setMenuState(settingsMenuView, 12, 5);
          break;
        
        case 5:
          whatToDisplay = 6;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("> Brightness");

          lcd.setCursor(1, 1);
          lcd.print("Sound");

          lcd.setCursor(15, 0);
          lcd.print("^");
          
          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW){
            brightnessItem = 0;
            settingsMenuView = 11; 
            swState = HIGH;
            lastSwState = HIGH;
          }
          
          settingsMenuView = setMenuState(settingsMenuView, 4, 3);
          break;
        
        case 6:
          whatToDisplay = 0;
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Reset Scores");

          lcd.setCursor(0, 1);
          lcd.print("> Go Back");

          lcd.setCursor(15, 0);
          lcd.print("^");
          delay(50);

          if (swState == LOW){
            menuState = 0;
            swState = HIGH;
            lastSwState = HIGH;
          }
          
          settingsMenuView = setMenuState(settingsMenuView, settingsMenuView, 7);
          break;
        
        case 7:
          whatToDisplay = 0;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("> Reset Scores");

          lcd.setCursor(1, 1);
          lcd.print("Go Back");
          
          lcd.setCursor(15, 0);
          lcd.print("^");
          delay(50);

          if (swState == LOW) {
            settingsMenuView = 14;
            swState = HIGH;
            lastSwState = HIGH;
          }
          
          settingsMenuView = setMenuState(settingsMenuView, 6, 13);
          break;

        case 8: // Name - Add / Edit
          whatToDisplay = 0;

          hasNameSet = true;
          setName();

          // yJoystickPosition: 0-4 letters of the name, 5 save
          if (swState == LOW && yJoystickPosition == 5) {
            hasChosenSaveOption = true;
            settingsMenuView = 0;
            swState = HIGH;
            lastSwState = HIGH;
          }

          break;

        case 9: // Difficulty
          whatToDisplay = 0;
          
          setDifficulty();
          if (swState == LOW) {
            settingsMenuView = 1;
            swState = HIGH;
            lastSwState = HIGH;
          }

          break;
        
        case 10: // Sound On/Off
          whatToDisplay = 4;
          lcd.createChar(0, speaker);
          lcd.createChar(1, soundOn);
          lcd.createChar(2, soundOff);

          setSound();
          if (swState == LOW) { 
            EEPROM.put(2, activateSound); // Save Sound preferences to EEPROM
            settingsMenuView = 13;
            swState = HIGH;
            lastSwState = HIGH;
          }
          
          break;
        
        case 11: // Brightness Matrix/LCD 
          whatToDisplay = 6;
          lcd.createChar(0, brightnessBar);

          setBrightness();
          if (swState == LOW) { 
            // Go one menu up and save settings for Display brightness
            EEPROM.put(0, lcdBrightness);
            EEPROM.put(1, matrixBrightness);
            settingsMenuView = 5;
            swState = HIGH;
            lastSwState = HIGH;
          }

          break;

        case 12:
          whatToDisplay = 0;
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Sound");

          lcd.setCursor(0, 1);
          lcd.print("> Reset Scores");

          lcd.setCursor(15, 0);
          lcd.print("^");

          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW){
            settingsMenuView = 14; 
            swState = HIGH;
            lastSwState = HIGH;
          }
          
          settingsMenuView = setMenuState(settingsMenuView, 6, 13);
          break;
        
        case 13:
          whatToDisplay = 4;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("> Sound");

          lcd.setCursor(1, 1);
          lcd.print("Reset Scores");

          lcd.setCursor(15, 0);
          lcd.print("^");
          
          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW){
            settingsMenuView = 10;
            swState = HIGH;
            lastSwState = HIGH;
          }
          
          settingsMenuView = setMenuState(settingsMenuView, 12, 5);
          break;
        
        case 14:
          whatToDisplay = 0;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("  Leaderboard");

          lcd.setCursor(1, 1);
          lcd.print("   Erased!");
          
          delay(50);
          
          if(noExistentHighscores != 0) {
            noExistentHighscores = 0;
            EEPROM.put(19, noExistentHighscores);
          }

          if (swState == LOW){
            settingsMenuView = 7; 
            swState = HIGH;
            lastSwState = HIGH;
          }
          
          break;
      }
    } else if (menuState == 4) {

      switch (infoMenuView) {
        case 0:
          whatToDisplay = 5;
          lcd.clear();
          displayText("Dev: Necula Narcis    ", 1, 1, false);
          displayText("Game: Snake", 1, 0, true);
          delay(50);

          infoMenuView = setMenuState(infoMenuView, 1, infoMenuView);

          break;
        
        case 1:
          whatToDisplay = 5;
          lcd.clear();
          displayText("Game: Snake", 1, 0, false);
          displayText("Dev: Necula Narcis    ", 1, 1, true);
          delay(50);

          infoMenuView = setMenuState(infoMenuView, 2, 0);

          break;
        
        case 2:
          whatToDisplay = 5;
          lcd.clear();
          displayText("Dev: Necula Narcis   ", 1, 0, false);
          displayText("GitHub: https://github.com/Narcis22    ", 1, 1, true);
          delay(50);

          infoMenuView = setMenuState(infoMenuView, 4, 3);
          
          break;
        
        case 3:
          whatToDisplay = 5;
          lcd.clear();
          displayText("GitHub: https://github.com/Narcis22     ", 1, 1, false);
          displayText("Dev: Necula Narcis    ", 1, 0, true);
          delay(50);

          infoMenuView = setMenuState(infoMenuView, 2, 0);

          break;
        
        case 4:
          whatToDisplay = 5;
          lcd.clear();
          displayText("GitHub: https://github.com/Narcis22     ", 1, 0, false);
          displayText("Intro to Robotics course, University of Bucharest.    ", 1, 1, true);
          delay(50);

          infoMenuView = setMenuState(infoMenuView, 6, 5);

          break;

        case 5:
          whatToDisplay = 5;
          lcd.clear();
          displayText("Intro to Robotics course, University of Bucharest.    ", 1, 1, false);
          displayText("GitHub: https://github.com/Narcis22     ", 1, 0, true);
          delay(50);

          infoMenuView = setMenuState(infoMenuView, 4, 3);
          
          break;

        case 6:
          whatToDisplay = 5;
          infoMenuView = setMenuState(infoMenuView, infoMenuView, 7);

          lcd.clear();
          displayText("Intro to Robotics course, University of Bucharest.    ", 1, 0, false);
          displayText("Go back", 1, 1, true);          
          delay(50);

          if (swState == LOW){
            menuState = 0;
            swState = HIGH;
            lastSwState = HIGH;
          }

          break;
        
        case 7:
          whatToDisplay = 5;
          lcd.clear();
          displayText("Go back", 1, 1, false);
          displayText("Intro to Robotics course, University of Bucharest.    ", 1, 0, true);
          delay(50);

          infoMenuView = setMenuState(infoMenuView, 6, 5);

          break;
      }
      
    } else if (menuState == 5) { // HOW TO PLAY 
      switch (htpMenuView) {
        case 0:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Snake game,");

          lcd.setCursor(0, 1);
          lcd.print("levels increase");
          
          lcd.setCursor(15, 1);
          lcd.write((byte)7);

          delay(50);

          htpMenuView = setMenuState(htpMenuView, 1, htpMenuView); 

          break;
        
        case 1:
          whatToDisplay = 5;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("speed after the");
          
          lcd.setCursor(15, 0);
          lcd.print("^");

          lcd.setCursor(0, 1);
          lcd.print("snake eats 55");

          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          
          delay(50);

          htpMenuView = setMenuState(htpMenuView, 2, 0); 

          break;
        
        case 2:
          whatToDisplay = 5;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("squares in ");

          lcd.setCursor(15, 0);
          lcd.print("^");

          lcd.setCursor(0, 1);
          lcd.print("length and wall");

          lcd.setCursor(15, 1);
          lcd.write((byte)7);

          delay(50);

          htpMenuView = setMenuState(htpMenuView, 3, 1); 

          break;
        
        case 3:
          whatToDisplay = 5;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("warping is");

          lcd.setCursor(15, 0);
          lcd.print("^");

          lcd.setCursor(0, 1);
          lcd.print("impossible in");

          lcd.setCursor(15, 1);
          lcd.write((byte)7);

          delay(50);

          htpMenuView = setMenuState(htpMenuView, 4, 2); 

          break;

        case 4:
          whatToDisplay = 5;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("lvl 6. Each lvl");

          lcd.setCursor(15, 0);
          lcd.print("^");

          lcd.setCursor(0, 1);
          lcd.print("starts in the");
          
          lcd.setCursor(15, 1);
          lcd.write((byte)7);

          delay(50);
          
          htpMenuView = setMenuState(htpMenuView, 5, 3); 

          break;

        case 5:
          whatToDisplay = 5;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("top-left corner");

          lcd.setCursor(15, 0);
          lcd.print("^");

          lcd.setCursor(0, 1);
          lcd.print("Click to exit.");

          delay(50);

          htpMenuView = setMenuState(htpMenuView, htpMenuView, 4); 

          break;
      }   

      if (swState == LOW){
        menuState = 0;
        swState = HIGH;
        lastSwState = HIGH;
      }

    }
  }
}

int setMenuState(int stay, int goDown, int goUp) {

  if (xValue < minThreshold && !xJoystickState) {
    indexText = 1;  // For a list with scrollable text
    xJoystickState = true;

    playNavigationSound();

    return goDown;
  }

  if (xValue > maxThreshold && !xJoystickState) {
    indexText = 1;
    xJoystickState = true;

    playNavigationSound();

    return goUp;
  }

  if (xValue < maxThreshold && xValue > minThreshold) {
    xJoystickState = false;
    return stay;
  }
}

void setName() {
  lcd.clear();

  lcd.setCursor(1, 0);
  lcd.print(playerName); 

  lcd.setCursor(12, 0);
  lcd.print("save"); 

  if (yJoystickPosition < 5) {
    // indication for the current letter
    lcd.setCursor(1 + yJoystickPosition, 1);
    lcd.print("^");  
  } else {
    // indication for the save button
    lcd.setCursor(11, 0);
    lcd.print(">");  
  }
  delay(50);

  movingBetweenPositions();
  if (yJoystickPosition < 5) {
    movingBetweenLetters();
  }
}

void movingBetweenLetters() {
  if (xValue < minThreshold && yValue > minThreshold && yValue < maxThreshold && !xJoystickState) {
    if (playerName[yJoystickPosition] != 'A') {
      playerName[yJoystickPosition] = (char)(playerName[yJoystickPosition] - 1);
    } else {
      playerName[yJoystickPosition] = 'Z';
    }

    xJoystickState = true;
  }
  
  if (xValue > maxThreshold && yValue > minThreshold && yValue < maxThreshold && !xJoystickState) {
    if (playerName[yJoystickPosition] != 'Z') {
      playerName[yJoystickPosition] = (char)(playerName[yJoystickPosition] + 1);
    } else {
      playerName[yJoystickPosition] = 'A';
    }

    xJoystickState = true;
  }

  if (xValue < maxThreshold && xValue > minThreshold) {
    xJoystickState = false;
  }
}

void movingBetweenPositions() {

  if (yValue < minThreshold && xValue > minThreshold && xValue < maxThreshold && !yJoystickState) {
    yJoystickState = true;
    yJoystickPosition = yJoystickPosition > 0 ? yJoystickPosition - 1 : 0;
  }

  if (yValue > maxThreshold && xValue > minThreshold && xValue < maxThreshold && !yJoystickState) {
    yJoystickState = true;
    yJoystickPosition = yJoystickPosition < 5 ? yJoystickPosition + 1  : 5;
  }

  if (yValue < maxThreshold && yValue > minThreshold) {
    yJoystickState = false;
  }
}

void setDifficulty() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("NOOB MEDIUM HELL");

  if (difficulty == 0 || difficulty == 1) {
    lcd.setCursor(1, 1);
  } else if (difficulty == 2 || difficulty == 3) {
    lcd.setCursor(7, 1);
  } else if (difficulty == 4 || difficulty == 5 || difficulty == 6) {
    lcd.setCursor(13, 1);
  }
  
  lcd.write("^");
  delay(50);

  movingSelectedDifficulty();
}

void movingSelectedDifficulty() {

  if (yValue < minThreshold && xValue > minThreshold && xValue < maxThreshold && !yJoystickState) {
    yJoystickState = true;
    
    if (difficulty == 0 || difficulty == 1) {
      difficulty = 4;
    } else if (difficulty == 2 || difficulty == 3) {
      difficulty = 0;
    } else if (difficulty == 4 || difficulty == 5 || difficulty == 6) {
      difficulty = 2;
    } 
  }

  if (yValue > maxThreshold && xValue > minThreshold && xValue < maxThreshold && !yJoystickState) {
    yJoystickState = true;
    
    if (difficulty == 4 || difficulty == 5 || difficulty == 6) {
      difficulty = 0;
    } else if (difficulty == 2 || difficulty == 3) {
      difficulty = 4;
    } else if (difficulty == 0 || difficulty == 1) {
      difficulty = 2;
    }
  }

  if (yValue < maxThreshold && yValue > minThreshold) {
    yJoystickState = false;
  }
}

void setSound() {
  // writing the custom speaker characters to the LCD 

  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.write((byte)0);
  lcd.write((byte)1); // Volume active

  lcd.setCursor(4, 1);
  lcd.write((byte)0);
  lcd.write((byte)2); // Volume disabled

  if (activateSound) {
    whatToDisplay = 4;
    lcd.setCursor(2, 0);
    lcd.print(">");   
  } else {
    whatToDisplay = 7;
    lcd.setCursor(2, 1);
    lcd.print(">");
  }
  delay(50);

  // Move the arrow above the wanted option
  if (xValue < minThreshold && !xJoystickState) {
    xJoystickState = true;
    activateSound = false;
  }

  if (xValue > maxThreshold && !xJoystickState) {
    xJoystickState = true;
    activateSound = true;
  }

  if (xValue < maxThreshold && xValue > minThreshold) {
    xJoystickState = false;
  }
}

void setBrightness() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Brightness for:");

  lcd.setCursor(8, 1);
  lcd.print("<");

  lcd.setCursor(14, 1);
  lcd.print(">");

  lcd.setCursor(1, 1);
  if (brightnessItem == 0){
    lcd.print("LCD"); 
    analogWrite(lcdBrightnessControlPin, lcdBrightness);   
  } else {
    lcd.print("Matrix");
    lc.setIntensity(0, matrixBrightness);
  }

  setBrightnessDisplay();
  delay(50);
  
  changeDisplayedBrightnessLevel();
  
  // Movement between the two available sliders
  if (xValue < minThreshold && yValue > minThreshold && yValue < maxThreshold && !xJoystickState) {
    xJoystickState = true;

    if (brightnessItem == 0) {
      brightnessItem = 1;
    }
  }

  if (xValue > maxThreshold && yValue > minThreshold && yValue < maxThreshold && !xJoystickState) {
    xJoystickState = true;
    
    if (brightnessItem == 1) {
      brightnessItem = 0;
    }  
  }

  if (xValue < maxThreshold && xValue > minThreshold) {
    xJoystickState = false;
  }
}

void setBrightnessDisplay() {
  byte displayedBrightnessLevel;
  
  if (brightnessItem == 0) {
    displayedBrightnessLevel = lcdBrightness / 51;
  } else {
    displayedBrightnessLevel = matrixBrightness / 3;
  }

  for (int i = 0 ; i < displayedBrightnessLevel; ++i) {
    lcd.setCursor(9 + i, 1);
    lcd.write((byte)0);
  }
}

// LCD range: 0-255 (6 increments of 51)
// Matrix range: 0-15 (6 increments of 3)
void changeDisplayedBrightnessLevel() {
  if (yValue < minThreshold && xValue > minThreshold && xValue < maxThreshold && !yJoystickState) {
    yJoystickState = true;

    if (brightnessItem == 0) {
      if (lcdBrightness != 0) {
        lcdBrightness -= 51;
      }
    } else {
      if (matrixBrightness != 0) {
        matrixBrightness -= 3;
      }
    }
  }

  if (yValue > maxThreshold && xValue > minThreshold && xValue < maxThreshold && !yJoystickState) {
    yJoystickState = true;
    if (brightnessItem == 0) {
      if (lcdBrightness != 255) {
        lcdBrightness += 51;
      }
    } else {
      if (matrixBrightness != 15) {
        matrixBrightness += 3;
      }
    }
  }

  if (yValue < maxThreshold && yValue > minThreshold) {
    yJoystickState = false;
  }
}

void displayText(String text, byte column, byte row, bool scrollable) {
  
  if (!scrollable) {

    lcd.setCursor(column, row);
    lcd.print(text.substring(0, 15));
  } else { 

    lcd.setCursor(column - 1, row);
    lcd.print(">");

    lcd.setCursor(column, row);
    
    if (text.length() <= 15) {
      lcd.print(text); 
    } else {
      String newText = text + "  ";

      if (indexText <= newText.length()) {
        scrollText(indexText, newText);
        indexText++;
      } else {
        indexText = 1;
      }
    }
  } 
}

// standard scroll text function
void scrollText(int startLetter, String message) {

  if (message.length() - startLetter > 15) {
    lcd.print(message.substring(startLetter, startLetter + 15));
  }
  else {    
    lcd.print(message.substring(startLetter, message.length()) + message.substring(0, 15 - message.length() + startLetter));
  } 

  delay(250);
}

// Initialization function for EEPROM
// Hard reset and rebase info
// It has to be called manually
void initializeEEPROM() {
  lcdBrightness = 102;
  matrixBrightness = 15;
  activateSound = true;
  noExistentHighscores = 0;

  EEPROM.put(0, lcdBrightness);
  EEPROM.put(1, matrixBrightness);
  EEPROM.put(2, activateSound);
  EEPROM.put(19, noExistentHighscores);
}

void setDataFromEEPROM() {
  lcdBrightness = EEPROM.read(0);
  matrixBrightness = EEPROM.read(1);
  activateSound = EEPROM.read(2);
}

void getLeaderboardFromEEPROM() {
  noExistentHighscores = EEPROM.read(19);
  delay(50);
  
  for (int i = 0; i < noExistentHighscores; ++i) {
    String currentPlayerName = "";
    long currentScore = 0;
  
    for (int j = 0; j < 5; ++j) {
      currentPlayerName += (char)EEPROM.read((i + 1) * 10 + j);
      delay(50);
    }

    byte firstByte = EEPROM.read((i + 1) * 10 + 6);
    delay(50);
    
    byte secondByte = EEPROM.read((i + 1) * 10 + 7);
    delay(50);

    currentScore = (firstByte * 256) + secondByte; // split in two for larger numbers, score supported until 65536

    leaderboardNames[i] = currentPlayerName;
    leaderboardScores[i] = currentScore;
  }
}

void applesCreate() {
  if (millis() - lastBlink > blinkTimeOn) {      
    lc.setLed(0, applePosition.x, applePosition.y, true);
  } 

  if (millis() - lastBlink > 2 * blinkTimeOn) {      
    lc.setLed(0, applePosition.x, applePosition.y, false);
    lastBlink = millis();
  }

  if (snakeBody[snakeLength - 1].x == applePosition.x && snakeBody[snakeLength - 1].y == applePosition.y ) {
    hasEatenApple = true;

    applePosition.x = random(matrixSize);
    applePosition.y = random(matrixSize);

    while (matrixLayout[applePosition.x][applePosition.y] == 1) {
      applePosition.x = random(matrixSize);
      applePosition.y = random(matrixSize);
    }

    if(sesionDifficulty == 0 || sesionDifficulty == 1) {
      score += 1;
    } else if(sesionDifficulty == 2 || sesionDifficulty == 3) {
      score += 3;
    } else if(sesionDifficulty == 4 || sesionDifficulty == 5 || sesionDifficulty == 6) {
      score += 5;
    }
  }
}

void changeDirection() {

  // Move Up
  if (xValue > maxThreshold && !xJoystickState) {
    xJoystickState = true;
    yJoystickState = true; 
    if (direction != 1) {
      direction = 3;
    }
  }

  // Move Down
  if (xValue < minThreshold && !xJoystickState) {
    xJoystickState = true;
    yJoystickState = true;
    if (direction != 3) {
      direction = 1;
    }
  }

  // Move left
  if (yValue < minThreshold && !yJoystickState) {
    yJoystickState = true;
    xJoystickState = true;
    if (direction != 0) {
      direction = 2;
    }
  }

  // Move right
  if (yValue > maxThreshold && !yJoystickState) {
    yJoystickState = true; 
    xJoystickState = true;
    if (direction != 2) {
      direction = 0;
    }
  }

  if (xValue < maxThreshold && xValue > minThreshold && yValue < maxThreshold && yValue > minThreshold) {
    xJoystickState = false;
    yJoystickState = false;
  }
}

void snakeMovement() {
  // Moving the body
  if (sesionDifficulty < 6) {
    if (hasEatenApple) {
      hasEatenApple = false;
      ++snakeLength;

      appleEatenYumYumSound();
        
      if (direction == 0) {
        snakeBody[snakeLength - 1].x = snakeBody[snakeLength - 2].x;
        snakeBody[snakeLength - 1].y = snakeBody[snakeLength - 2].y + 1;

        if (snakeBody[snakeLength - 1].y > 7) {
          snakeBody[snakeLength - 1].y = 0;
        }
      } else if(direction == 1) {
        snakeBody[snakeLength - 1].x = snakeBody[snakeLength - 2].x + 1;
        snakeBody[snakeLength - 1].y = snakeBody[snakeLength - 2].y;

        if (snakeBody[snakeLength - 1].x > 7) {
          snakeBody[snakeLength - 1].x = 0;
        }
      } else if(direction == 2) {
        snakeBody[snakeLength - 1].x = snakeBody[snakeLength - 2].x;
        snakeBody[snakeLength - 1].y = snakeBody[snakeLength - 2].y - 1;

        if (snakeBody[snakeLength - 1].y == -1 || snakeBody[snakeLength - 1].y == 255) {
          snakeBody[snakeLength - 1].y = 7;
        }
      } else if(direction == 3) {
        snakeBody[snakeLength - 1].x = snakeBody[snakeLength - 2].x - 1;
        snakeBody[snakeLength - 1].y = snakeBody[snakeLength - 2].y;

        if (snakeBody[snakeLength - 1].x == -1 || snakeBody[snakeLength - 1].x == 255) {
          snakeBody[snakeLength - 1].x = 7;
        }
      }

      if (matrixLayout[snakeBody[snakeLength - 1].x][snakeBody[snakeLength - 1].y] == 1 || matrixLayout[snakeBody[snakeLength - 1].x][snakeBody[snakeLength - 1].y] == 3){
        checkLives();
      }

      lc.setLed(0, snakeBody[snakeLength - 1].x, snakeBody[snakeLength - 1].y, true);
      matrixLayout[snakeBody[snakeLength - 1].x][snakeBody[snakeLength - 1].y] = 1;

    } else {
  
      lc.setLed(0, snakeBody[0].x, snakeBody[0].y, false);
      matrixLayout[snakeBody[0].x][snakeBody[0].y] = 0;

      for (indexMove = 0; indexMove < snakeLength - 1; ++indexMove){
        snakeBody[indexMove] = snakeBody[indexMove + 1];
      }

      if (direction == 0) {
        snakeBody[snakeLength - 1].x = snakeBody[snakeLength - 2].x;
        snakeBody[snakeLength - 1].y = snakeBody[snakeLength - 2].y + 1;

        if (snakeBody[snakeLength - 1].y > 7) {
          snakeBody[snakeLength - 1].y = 0;
        }
      } else if(direction == 1) {
        snakeBody[snakeLength - 1].x = snakeBody[snakeLength - 2].x + 1;
        snakeBody[snakeLength - 1].y = snakeBody[snakeLength - 2].y;

        if (snakeBody[snakeLength - 1].x > 7) {
          snakeBody[snakeLength - 1].x = 0;
        }
      } else if(direction == 2) {
        snakeBody[snakeLength - 1].x = snakeBody[snakeLength - 2].x;
        snakeBody[snakeLength - 1].y = snakeBody[snakeLength - 2].y - 1;
        
        if (snakeBody[snakeLength - 1].y == -1 || snakeBody[snakeLength - 1].y == 255) {
          snakeBody[snakeLength - 1].y = 7;
        }
      } else if(direction == 3) {
        snakeBody[snakeLength - 1].x = snakeBody[snakeLength - 2].x - 1;
        snakeBody[snakeLength - 1].y = snakeBody[snakeLength - 2].y;

        if (snakeBody[snakeLength - 1].x == -1 || snakeBody[snakeLength - 1].x == 255) {
          snakeBody[snakeLength - 1].x = 7;
        }
      }

      if (matrixLayout[snakeBody[snakeLength - 1].x][snakeBody[snakeLength - 1].y] == 1 || matrixLayout[snakeBody[snakeLength - 1].x][snakeBody[snakeLength - 1].y] == 3){
        checkLives();
      }

      lc.setLed(0, snakeBody[snakeLength - 1].x, snakeBody[snakeLength - 1].y, true);
      matrixLayout[snakeBody[snakeLength - 1].x][snakeBody[snakeLength - 1].y] = 1;
    }
  } else if(sesionDifficulty == 6) {
      if (hasEatenApple) {
        hasEatenApple = false;
        ++snakeLength;

        appleEatenYumYumSound();

        if (direction == 0) {
          snakeBody[snakeLength - 1].x = snakeBody[snakeLength - 2].x;
          snakeBody[snakeLength - 1].y = snakeBody[snakeLength - 2].y + 1;

          if (snakeBody[snakeLength - 1].y > 7) {
            checkLives();
          }
        } else if(direction == 1) {
          snakeBody[snakeLength - 1].x = snakeBody[snakeLength - 2].x + 1;
          snakeBody[snakeLength - 1].y = snakeBody[snakeLength - 2].y;

          if (snakeBody[snakeLength - 1].x > 7) {
            checkLives();
          }
        } else if(direction == 2) {
          snakeBody[snakeLength - 1].x = snakeBody[snakeLength - 2].x;
          snakeBody[snakeLength - 1].y = snakeBody[snakeLength - 2].y - 1;

          if (snakeBody[snakeLength - 1].y == -1 || snakeBody[snakeLength - 1].y == 255) {
            checkLives();
          }
        } else if(direction == 3) {
          snakeBody[snakeLength - 1].x = snakeBody[snakeLength - 2].x - 1;
          snakeBody[snakeLength - 1].y = snakeBody[snakeLength - 2].y;

          if (snakeBody[snakeLength - 1].x == -1 || snakeBody[snakeLength - 1].x == 255) {
            checkLives();
          }
        }

        if (matrixLayout[snakeBody[snakeLength - 1].x][snakeBody[snakeLength - 1].y] == 1 || matrixLayout[snakeBody[snakeLength - 1].x][snakeBody[snakeLength - 1].y] == 3){
          checkLives();
        }

        lc.setLed(0, snakeBody[snakeLength - 1].x, snakeBody[snakeLength - 1].y, true);
        matrixLayout[snakeBody[snakeLength - 1].x][snakeBody[snakeLength - 1].y] = 1;

      } else {
    
        lc.setLed(0, snakeBody[0].x, snakeBody[0].y, false);
        matrixLayout[snakeBody[0].x][snakeBody[0].y] = 0;

        for (indexMove = 0; indexMove < snakeLength - 1; ++indexMove){
          snakeBody[indexMove] = snakeBody[indexMove + 1];
        }

        if (direction == 0) {
          snakeBody[snakeLength - 1].x = snakeBody[snakeLength - 2].x;
          snakeBody[snakeLength - 1].y = snakeBody[snakeLength - 2].y + 1;

          if (snakeBody[snakeLength - 1].y > 7) {
            checkLives();
          }
        } else if(direction == 1) {
          snakeBody[snakeLength - 1].x = snakeBody[snakeLength - 2].x + 1;
          snakeBody[snakeLength - 1].y = snakeBody[snakeLength - 2].y;

          if (snakeBody[snakeLength - 1].x > 7) {
            checkLives();
          }
        } else if(direction == 2) {
          snakeBody[snakeLength - 1].x = snakeBody[snakeLength - 2].x;
          snakeBody[snakeLength - 1].y = snakeBody[snakeLength - 2].y - 1;
          
          if (snakeBody[snakeLength - 1].y == -1 || snakeBody[snakeLength - 1].y == 255) {
            checkLives();
          }
        } else if(direction == 3) {
          snakeBody[snakeLength - 1].x = snakeBody[snakeLength - 2].x - 1;
          snakeBody[snakeLength - 1].y = snakeBody[snakeLength - 2].y;

          if (snakeBody[snakeLength - 1].x == -1 || snakeBody[snakeLength - 1].x == 255) {
            checkLives();
          }
        }

        if (matrixLayout[snakeBody[snakeLength - 1].x][snakeBody[snakeLength - 1].y] == 1 || matrixLayout[snakeBody[snakeLength - 1].x][snakeBody[snakeLength - 1].y] == 3){
          checkLives();
        }

        lc.setLed(0, snakeBody[snakeLength - 1].x, snakeBody[snakeLength - 1].y, true);
        matrixLayout[snakeBody[snakeLength - 1].x][snakeBody[snakeLength - 1].y] = 1;
      }
    }

    if (snakeLength == 20) { // disputable
      levelUp();
    }

    if (snakeLength == 55) {
      playing = false;
    }
}

void displayInGameStats() {

  lcd.clear();
  lcd.setCursor(0, 0);

  if (hasNameSet) {
    // Name was set in the settings menu
    lcd.print(playerName);
  } else {
    // If there was no name introduced will show ANONI on the in-game menu displayed on the LCD
    lcd.print("ANONI");
  }
  
  lcd.setCursor(6, 0);
  lcd.print("Lvl: " + String(sesionDifficulty));

  lcd.setCursor(0, 1);

  for ( int i = 0; i < livesLeft; ++i) {
    lcd.setCursor(i, 1);
    lcd.write((byte)0);
  }
  
  lcd.setCursor(6, 1);
  lcd.print("EXP:" + String(score));
  delay(20);
}

void showGameEndedSequence() {
  lcd.clear();
  
  // Game ended, Displaying stats and asking for Name for Leaderboard
  if (millis() - startGameOverSequence < gameOverDuration) {
    lcd.setCursor(0, 0);
    lcd.print("   GAME OVER!");

    lcd.setCursor(0, 1);
    lcd.print("Your Score:" + String(score));
  } else {
    if (!scoreMarked) {
      
      insertPosition = 0;

      while (insertPosition < noExistentHighscores && score <= leaderboardScores[insertPosition]){
        ++insertPosition;
      }
      
      if (insertPosition < 5) {
        whatToDisplay = 3;
        if(!hasNameSet) {
            whatToDisplay = 3;
            setName();

            if (swState == LOW && yJoystickPosition == 5) {
              hasNameSet = true;
              swState = HIGH;
              lastSwState = HIGH;
            }
        } else {
          if (!hasChosenSaveOption) {
            whatToDisplay = 3;
            saveNameOption();

            if (swState == LOW){
              hasChosenSaveOption = true;
              swState = HIGH;
              lastSwState = HIGH;
            }
          } else { 
            if (insertPosition == noExistentHighscores) {
              noExistentHighscores += 1;
              leaderboardNames[insertPosition] = playerName;
              leaderboardScores[insertPosition] = score;

            } else {
              for (int i = noExistentHighscores - 1; i > insertPosition; --i) {
                leaderboardNames[i] = leaderboardNames[i - 1];
                leaderboardScores[i] = leaderboardScores[i - 1];
              } 

              leaderboardNames[insertPosition] = playerName;
              leaderboardScores[insertPosition] = score;
              if(noExistentHighscores < 5) {
                ++noExistentHighscores;
              }
            }

            saveLeaderboardToEEPROM();
            scoreMarked = true;

            if (!saveName) {
              playerName = "AAAAA";
              hasNameSet = false;
            }
          }
        }
      } else {
        scoreMarked = true;        
      }
    }
  }
}

void saveLeaderboardToEEPROM() {
  EEPROM.put(19, noExistentHighscores);

  for (int i = 1; i <= noExistentHighscores; ++i) {
      
    for (int j = 0; j < 5; ++j) {
      EEPROM.put(i * 10 + j, leaderboardNames[i - 1][j]);
    }

    int firstByte = highByte (leaderboardScores[i - 1]);
    int secondByte = lowByte (leaderboardScores[i - 1]); 

    EEPROM.put(i * 10 + 6, firstByte);
    EEPROM.put(i * 10 + 7, secondByte);
  }
}

void resetMatrix() {
  for (int i = 0; i < matrixSize; ++i) {
    for (int j = 0; j < matrixSize; ++j) {
      lc.setLed(0, i, j, true);
      delay(20);
    }
  }

  for (int i = 0; i < matrixSize; ++i) {
    for (int j = 0; j < matrixSize; ++j) {
      lc.setLed(0, i, j, false);
      delay(20);
    }
  }
}

void saveNameOption() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Save new name?");

  if (saveName) {
    lcd.setCursor(0, 1);
    lcd.print("   >Yes    No");
  } else {
    lcd.setCursor(0, 1);
    lcd.print("    Yes   >No");
  }
   
  // Move between options
  if (yValue < minThreshold && !yJoystickState) {
    yJoystickState = true;
    saveName = true;
  }

  if (yValue > maxThreshold && !yJoystickState) {
    yJoystickState = true;
    saveName = false;
  }

  if (yValue < maxThreshold && yValue > minThreshold) {
    yJoystickState = false;
  } 
}

// Display the custom images on the Matrix display
void printImageOnMatrix(const byte* image) {
  
  for (int i = 0; i < matrixSize; ++i) {
    for (int j = 0; j < matrixSize; ++j) {
      lc.setLed(0, i, j, false);
    }
  }
  
  for (int i = 0; i < matrixSize; ++i) {
    for (int j = 0; j < matrixSize; ++j) {
      lc.setLed(0, i, j, bitRead(image[i], 7 - j));
    }
  }
}

void levelUp() {
    if (sesionDifficulty < 6) {
      ++sesionDifficulty;

      lvlUpBuzz();
    
      snakeBody[0].x = 0;
      snakeBody[0].y = 0;
      snakeBody[1].x = 0;
      snakeBody[1].y = 1;
      snakeLength = 2;
      direction = 0;
      
      for (int i = 0; i < matrixSize; ++i) {
        for (int j = 0; j < matrixSize; ++j) {
          lc.setLed(0, i, j, false);
          matrixLayout[i][j] = 0;
        }
      }

      for (int i = 0; i < snakeLength; ++i) {
        lc.setLed(0, snakeBody[i].x, snakeBody[i].y, true);
        matrixLayout[snakeBody[i].x][snakeBody[i].y] = 1;
      }
    }
}

void checkLives() {
  if (livesLeft > 0) {
    --livesLeft;
    
    lostLifeSound();

    snakeBody[0].x = 0;
    snakeBody[0].y = 0;
    snakeBody[1].x = 0;
    snakeBody[1].y = 1;
    snakeLength = 2; 
    direction = 0;
    
    for (int i = 0; i < matrixSize; ++i) {
      for (int j = 0; j < matrixSize; ++j) {
        lc.setLed(0, i, j, false);
        matrixLayout[i][j] = 0;
      }
    }

    for (int i = 0; i < snakeLength; ++i) {
      lc.setLed(0, snakeBody[i].x, snakeBody[i].y, true);
      matrixLayout[snakeBody[i].x][snakeBody[i].y] = 1;
    }
  } else { 
    playing = false;
  }
} 

// Manually tuning a sound for Game Over
// Not original 
void gameOverSound() {
  if (activateSound) {
    tone(buzzer, 500, 200);
    delay(100);
    tone(buzzer, 1200, 200);
    delay(100);
    tone(buzzer, 300, 200);
    delay(100);
    tone(buzzer, 1000, 200);
    delay(100);
    tone(buzzer, 400, 200);
    delay(100);
    tone(buzzer, 100, 200);
    delay(100); 
  }
}

// Customize moving sound
void playNavigationSound() {
  if (activateSound) {
    tone(buzzer, 30, 40);
    delay(50);
  }
}

// Lost life sound
void lostLifeSound() {
  if (activateSound) {
    tone(buzzer, NOTE_E6, 150);
    delay(70);
    noTone(8);
  }
}

// lvl up sound
void lvlUpBuzz() {
  if (activateSound) {
    tone(buzzer, NOTE_E6, 55);
    delay(130);
    tone(buzzer, NOTE_G6, 55);
    delay(130);
    noTone(8);
  }
}

// apple eaten sound
void appleEatenYumYumSound() {
  if (activateSound) {
    tone(buzzer, NOTE_G4, 35);
    delay(35);
    noTone(8);  
  }
}


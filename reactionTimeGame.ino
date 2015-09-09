#include <MusicPlayer.h>
#include <Pitches.h>
#include "musicTrackDefinitions.h"

#include "lilypadPins.h"
#include "Light.h"
#include "Colors.h"

// Game States
#define GAME_START 1
#define MINI_WIN 2
#define MINI_LOSE 3
#define GAME_LOOP 4
#define PAUSE     5
#define GAME_OVER 6

#define GAME_UPDATE_SPEED 100 // update 20 times a second (1000/50)

int mainGameState = GAME_START;

// Constants
static const int MAX_TURNS = 2;
static const int REQ_FOR_WIN = 1;
static const int NUM_WHITE_LIGHTS = 3;
static const unsigned long GAME_ROUND_SPEEDS[8] = {300, 275, 250, 230, 220, 210, 200, 180};
static const int WIN_ROUND_TIME = 2000; // ms
static const int LOSE_ROUND_TIME = 2000; // ms
static const int WIN_COLOR = GREEN;
static const int LOSE_COLOR = SALMON;

float delayOffset = 0.75;

Light whiteLights[3] = {Light(WHITE_1), Light(WHITE_2), Light(WHITE_3)};
Light winLight = Light(WHITE_4); // todo hook up 4th light, or replace with 3rd light?
Light rgbLight = Light(0);
int currColor=-1;
int prevTime;

//MusicPlayer & SFX
MusicPlayer music(BUZZER_PIN);

// Vars to reinitialize every game
int currTurn = 0;
int numWins = 0;
unsigned long lightSpeed;


void setup() {
  Serial.begin(9600);      // open the serial port at 9600 bps for Debug Output: 
  rgbLight.setRGBPins(RGB_LED_RED, RGB_LED_GREEN, RGB_LED_BLUE);
  randomSeed(analogRead(0));
  // initialize the pushbutton pin as an input:
  pinMode(BUTTON, INPUT_PULLUP);
  
  initGameLoop();
}

void initGameLoop()
{
  mainGameState = PAUSE;
  numWins = 0;
  currTurn = 0;
  lightSpeed=GAME_ROUND_SPEEDS[0];
  music.init(true);
  continueGameLoop(); 
}

void continueGameLoop()
{
  mainGameState = PAUSE;
  startLightsArray(lightSpeed, GAME_ROUND_SPEEDS[currTurn]*delayOffset);
  
  setRandomRGB();
  mainGameState = GAME_LOOP;
}

void loop() {
    
  music.updateTrack();
  rgbLight.updateLED();
  
  switch(mainGameState)
  {
    case GAME_LOOP:
    {
      // if user is pressing the button
      if(digitalRead(BUTTON) == LOW)
      {
        if(currColor==WIN_COLOR)
        {
          Serial.write("mini-win\n");
          miniWin();
        }
        else // else color is not WIN_COLOR so you lose
        {
          Serial.write("mini-loss\n");
          miniLose();
        }
        return;
      }
      
      // loop thru lights & reset them if they're off
      int whiteLightsOff=0;
      for(int i=0; i<NUM_WHITE_LIGHTS; i++)
      {
        whiteLights[i].updateLED();
        if(!whiteLights[i].isActive()){
          whiteLightsOff++;
        }
        if(whiteLightsOff==NUM_WHITE_LIGHTS)
        {
          //restart lights
          startLightsArray(lightSpeed, lightSpeed*delayOffset);
          setRandomRGB();
          return;
        }
      }
     break;
      }
     case MINI_WIN:
     {
       // if we're done celebrating
       if(!rgbLight.isActive())
       {
         // decide if the game is over
        currTurn++;
        if(currTurn >= MAX_TURNS)
        {
          gameComplete();
          return;
        }
        
         //reset game at new speed
         lightSpeed = GAME_ROUND_SPEEDS[currTurn];
         continueGameLoop();
       }
     break;
     }
     case MINI_LOSE:
     {
       // if we're done losing
       if(!rgbLight.isActive())
       {
         // decide if the game is over
        currTurn++;
        if(currTurn >= MAX_TURNS)
        {
          gameComplete();
          return;
        }
        
         // reset game at slower speed
         lightSpeed = GAME_ROUND_SPEEDS[currTurn];
         continueGameLoop();
       }
     break;
     }
     case GAME_OVER:
     {
       if(music.isDonePlaying())
         initGameLoop();
     break;
     }
  }
}

void gameComplete()
{
  Serial.write("Game Complete");
  
  // decide if they won or lost
  if(numWins > REQ_FOR_WIN)
  {
    win();
  }
  else // they lost
  {
    lose();
  }
}
void startLightsArray(int onTime, int betweenTime)
{ 
  for(int i=0; i<NUM_WHITE_LIGHTS; i++)
  {
    whiteLights[i].whiteLEDOn(onTime, betweenTime*i);
  }
}

void setRandomRGB()
{
  // 30% of the time, it's GREEN
  // 70% of the time, it's a random color
  int newColor = random(10);
  if(newColor<3) // GREEN
    currColor = GREEN;
  else
    currColor = random(NUM_COLORS);

  rgbLight.rgbLEDOn(COLORS[currColor][0], COLORS[currColor][1], COLORS[currColor][2], lightSpeed*NUM_WHITE_LIGHTS*(1+delayOffset)-lightSpeed*delayOffset, 0);   
  //delay(100);
}

void miniWin()
{
  mainGameState = MINI_WIN;
  numWins++;
  setLightsOff();
  
  // show WIN_COLOR color
  rgbLight.rgbLEDOn(COLORS[WIN_COLOR][0], COLORS[WIN_COLOR][1], COLORS[WIN_COLOR][2], WIN_ROUND_TIME, 0);

  // play miniWin music
  music.playTrack(miniWinMusic, MINI_WIN_NUM);
}
void win()
{
  // play win music
  music.playTrack(starWars, STAR_WARS_NUM);
  mainGameState = GAME_OVER;
}
void lose()
{
  //music.playLose();
  music.playTrack(loseMusic, LOSE_MUSIC_NUM);
  mainGameState = GAME_OVER;
}
void setLightsOff()
{
  // turn off white leds
  for(int i=0; i<NUM_WHITE_LIGHTS; i++)
    whiteLights[i].setOff();
}

void miniLose()
{
  mainGameState = MINI_LOSE;
  setLightsOff();
  rgbLight.rgbLEDOn(COLORS[LOSE_COLOR][0], COLORS[LOSE_COLOR][1], COLORS[LOSE_COLOR][2], LOSE_ROUND_TIME, 0);
  
  // play lose music
  music.playTrack(miniLoseMusic, MINI_LOSE_NUM);
}


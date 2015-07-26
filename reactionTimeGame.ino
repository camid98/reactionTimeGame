#include "lilypadPins.h"
#include "Music.h"
#include "Light.h"

#define GAME_START 1
#define MINI_WIN 2
#define MINI_LOSE 3
#define GAME_LOOP 4
#define PAUSE 5
#define WIN 6
#define LOSE 7

static const int maxTurns = 8;
static const int reqForWin = 6;
static const int numWhiteLights = 3;
static const unsigned long gameSpeeds[8] = {300, 275, 250, 230, 220, 210, 200, 180};
float delayOffset = 0.75;

Light whiteLights[3] = {Light(WHITE_1), Light(WHITE_2), Light(WHITE_3)};

Light winLight = Light(WHITE_4);
Light rgbLight = Light(0);
                    // Sky Blue,     Magenta,       Green,         Red
int colors[4][3] = {{13, 155, 200}, {224, 24, 184}, {12, 252, 0}, {255, 0, 0}};
#define SKY_BLUE 0
#define PINK 1
#define GREEN 2
#define RED 3
int currColor=-1;

// winning
static const int miniWinTime = 2000;
static const int miniLoseTime = 2000;

//Music & SFX
Music music(BUZZER_PIN);

// Vars to reinitialize every game
int mainGameState = GAME_START;
int currTurn = 0;
int numWins = 0;
unsigned long startTime;
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
  startTime;
  lightSpeed=gameSpeeds[0];
  music.init();
  
  continueGameLoop(); 
}

void continueGameLoop()
{
  mainGameState = PAUSE;
  unsigned long delayx = gameSpeeds[currTurn]*delayOffset;
  startLightsArray(lightSpeed, delayx);
  Serial.write("light speed: ");
  Serial.println(lightSpeed);
  Serial.write("delayed by: ");
  Serial.println(delayx);
  Serial.write(" based on delayOffset being: ");
  Serial.println(delayOffset);
  
  setRandomRGB();
  mainGameState = GAME_LOOP;
}

void loop() {
  switch(mainGameState)
  {
    case GAME_LOOP:
    {
      // if user is pressing the button
      if(digitalRead(BUTTON) == LOW)
      {
        // pressing button
        // if green, you win
        if(currColor==GREEN)
        {
          Serial.write("mini-win\n");
          miniWin();
        }
        else // else color is not green so you lose
        {
          Serial.write("mini-loss\n");
          miniLose();
        }
        return;
      }
      
      rgbLight.updateLED();
      // loop thru lights & reset them if they're off
      int whiteLightsOff=0;
      for(int i=0; i<numWhiteLights; i++)
      {
        whiteLights[i].updateLED();
        if(!whiteLights[i].isActive()){
          whiteLightsOff++;
        }
        if(whiteLightsOff==numWhiteLights)
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
        if(currTurn >= maxTurns)
        {
          gameComplete();
          return;
        }
        
         //reset game at new speed
         lightSpeed = gameSpeeds[currTurn];
         continueGameLoop();
       }
       else {
         rgbLight.updateLED();
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
        if(currTurn >= maxTurns)
        {
          gameComplete();
          return;
        }
        
         // reset game at slower speed
         lightSpeed = gameSpeeds[currTurn];
         continueGameLoop();
       }
       else
         rgbLight.updateLED();
     break;
     }
  }
}

void gameComplete()
{
  Serial.write("Game Complete");
  
  // decide if they won or lost
  if(numWins > reqForWin)
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
  for(int i=0; i<numWhiteLights; i++)
  {
    whiteLights[i].whiteLEDOn(onTime, betweenTime*i);
  }
}

void setRandomRGB()
{
  currColor = random(3);
  rgbLight.rgbLEDOn(colors[currColor][0], colors[currColor][1], colors[currColor][2], lightSpeed*numWhiteLights*(1+delayOffset)-lightSpeed*delayOffset, 0);
  
  if(currColor == GREEN)
    startTime=millis();
  
   rgbLight.updateLED();
}

void miniWin()
{
  Serial.write("setting win state/n");
  mainGameState = MINI_WIN;
  numWins++;
  setLightsOff();
  
  // show green color
  rgbLight.rgbLEDOn(colors[GREEN][0], colors[GREEN][1], colors[GREEN][2], miniWinTime, 0);
  music.playUp();
}
void win()
{
  // play win music
  music.playWin();
  initGameLoop();
}
void lose()
{
  music.playLose();
  initGameLoop();
}
void setLightsOff()
{
  // turn off white leds
  for(int i=0; i<numWhiteLights; i++)
    whiteLights[i].setOff();
}

void miniLose()
{
  mainGameState = MINI_LOSE;
  setLightsOff();
  rgbLight.rgbLEDOn(colors[RED][0], colors[RED][1], colors[RED][2], miniLoseTime, 0);
  // play lose music
  music.playDown();
}
void debugOutputLights()
{
    // dump debug info
    for(int p=0; p<numWhiteLights; p++)
    {
      Serial.write("light # ");
      Serial.print(p, DEC);
      Serial.write(" =======\n");
      whiteLights[p].debugOutput();
      Serial.write("\n");
    }
}

#include "Light.h"
#include "Arduino.h"

Light::Light(int whiteLED)
{
  pinMode(whiteLED, OUTPUT);
  this->_pin = whiteLED;
}
void Light::setRGBPins(int rPin, int gPin, int bPin)
{
  _rPin = rPin;
  _gPin = gPin;
  _bPin = bPin;
}
void Light::whiteLEDOn(unsigned long onTime, unsigned long delayStart)
{
  _isRGB=false;
  LEDOn(onTime, delayStart);
}

void Light::rgbLEDOn(int r, int g, int b, unsigned long onTime, unsigned long delayStart)
{
  _r = r;
  _g = g;
  _b = b;
  _isRGB = true;
  LEDOn(onTime, delayStart);
}

void Light::LEDOn(unsigned long onTime, unsigned long delayStart)
{  
   time_diff = onTime;
   time_delay_start = delayStart;
   time_start = millis();
   _isActive=true;
   
   if(time_delay_start > 0)
   {
     _isDelayed=true;
   }
   else 
   {
     setOn();
   }
}

void Light::setOn()
{
   _isOn = true;
   _isDelayed=false;
   if(_isRGB)
   {
     Serial.write("setting rgb on: \n");
     Serial.println(_r, DEC);
     Serial.println(_g, DEC);
     Serial.println(_b, DEC);
     Serial.write("\n");
     analogWrite(_rPin, 255 - _r);
     analogWrite(_gPin, 255 - _g);
     analogWrite(_bPin, 255 - _b);
   }
   else // is white light
   {
     digitalWrite(_pin, HIGH);   // turn the LED on (HIGH is the voltage level);
   }
   time_start = millis();
}

void Light::setOff()
{
  _isOn = false;
  _isActive=false;
  if(_isRGB)
  {
    Serial.write("settign color light off\n");
    analogWrite(_rPin, 255);
    analogWrite(_gPin, 255);
    analogWrite(_bPin, 255);
  }
  else // white LED
  {
    digitalWrite(_pin, LOW);
  }
}

void Light::updateLED()
{
  if(_isActive)
  {
    if(_isDelayed && ((millis() - time_start) >= time_delay_start))
    {
      setOn();
    }
    else if(_isOn && ((millis() - time_start) >= time_diff))
    {
      // light is done can turn off now
      setOff();
    }
  }
}

bool Light::isActive()
{
  return _isActive;
}

void Light::debugOutput()
{
    Serial.write(": isActive");
    Serial.print(_isActive, BIN);
    Serial.write(", _isAOn: ");
    Serial.print(_isOn, BIN);
    Serial.write(", _isDelayed: ");
    Serial.print(_isDelayed, BIN);
    Serial.write(", time_start: ");
    Serial.print(time_start, DEC);
    Serial.write(", time_delay_start");
    Serial.print(time_delay_start, DEC);  
}

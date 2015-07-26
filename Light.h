#ifndef Light_h
#define Light_h


class Light
{
  public:
    Light(int whiteLED);
    void setRGBPins(int rPin, int gPin, int bPin);
    void whiteLEDOn(unsigned long onTime, unsigned long delayStart);
    void rgbLEDOn(int r, int g, int b, unsigned long onTime, unsigned long delayStart);
    void LEDOn(unsigned long onTime, unsigned long delayStart);
    void updateLED();
    void setOn();
    void setOff();
    void debugOutput();
    bool isActive();
  private:
    int _pin;   
    bool _isOn=false; 
    bool _isActive=false; 
    bool _isDelayed=false;
    bool _isRGB=false;
    int _rPin;
    int _gPin;
    int _bPin;
    int _r;
    int _g;
    int _b;
    unsigned long time_start;
    unsigned long time_delay_start; 
    unsigned long time_diff;
};

#endif

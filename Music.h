#ifndef Music_h
#define Music_h

#include "Arduino.h"
#include "pitches.h"

class Music
{
  public:
    Music(int pin);
    void init();
    void playWin();
    void playLose();
    void playUp();
    void playDown();
    void playSound(int noteIndex);
  private:
    int _pin;    
    int currSFXID=0;
    // notes in the melody:
    static const int win_length = 19;
    int win_melody[win_length] = {
      NOTE_C4, NOTE_C4, NOTE_C4, NOTE_C4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_D4, 
      NOTE_C5, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_D4,
      NOTE_C5, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_F4, NOTE_D4
    };
    // note durations: 4 = quarter note, 8 = eighth note, etc.:
    int win_noteDuration[win_length] = {
      2, 2, 2, 8, 8, 2, 2, 2, 
      8, 8, 2, 2, 2, 
      8, 8, 2, 2, 2, 8
    };
    int win_pauseDuration[win_length-1] = {
      2, 2, 2, 2, 2, 2, 2, 2, 
      2, 2, 2, 2, 2,
      2, 2, 2, 2, 2
    };
    static const int lose_length = 9;
    int lose_melody[lose_length] = {
      NOTE_C3, NOTE_A2, 
      NOTE_B2, NOTE_G2, 
      NOTE_A2, NOTE_F2, 
      NOTE_G2, NOTE_D2, NOTE_C2
    };
    // note durations: 4 = quarter note, 8 = eighth note, etc.:
    int lose_noteDuration[lose_length] = {
      4, 4, 
      4, 4, 
      4, 4, 
      4, 8, 8
    };
    int lose_pauseDuration[lose_length-1] = {
      4, 4, 
      4, 4,
      4, 4, 
      8, 8
    };
    static const int sfx_length = 8;
    int sfx_scale[sfx_length] = {
      NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5
    };
};

#endif

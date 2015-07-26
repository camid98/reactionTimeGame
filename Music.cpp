#include "Arduino.h"
#include "Music.h"
#include "pitches.h"

Music::Music(int pin)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
}
void Music::init()
{
  currSFXID = 0;
}

void Music::playWin()
{
  Serial.write("win music start\n");
  for (int thisNote = 0; thisNote < win_length; thisNote++) {
    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int currNoteDuration = 1000 / (10-win_noteDuration[thisNote]);
    
    tone(_pin, win_melody[thisNote], currNoteDuration);
    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = currNoteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(_pin);
    // delay before the next note is played
    delay(win_pauseDuration[thisNote]+1);
  }
  Serial.write("win music done\n");
}

void Music::playLose()
{
  Serial.write("lose music start\n");

  for (int thisNote = 0; thisNote < lose_length; thisNote++) {
    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int currNoteDuration = 1000 / (10-lose_noteDuration[thisNote]);
    
    tone(_pin, lose_melody[thisNote], currNoteDuration);
    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = currNoteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(_pin);
    // delay before the next note is played
    delay(lose_pauseDuration[thisNote]+1);
  }
  Serial.write("lose music done\n");
}

void Music::playUp()
{
  playSound(currSFXID);
  currSFXID++;
  if(currSFXID>=sfx_length)
    currSFXID = 0;
  playSound(currSFXID);
}
void Music::playDown()
{
  playSound(currSFXID);
  currSFXID--;
  if(currSFXID<0)
    currSFXID = sfx_length-1;
  playSound(currSFXID);

}
void Music::playSound(int noteIndex)
{
  tone(_pin, sfx_scale[noteIndex]);
  delay(500);
  // stop the tone playing:
  noTone(_pin);
}

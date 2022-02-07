/*************************************************** 
  ProtoStax Mml Music Player

  The ProtoStax_MmlMusicPlayer library extends the MmlMusic library to provide asynchronous playback 
  capabilities using all available Timers on the Arduino. 
  This has been tested with the Arduino Uno and uses Timer 1 and Timer 2. 

  Muliple ProtoStax_MmlMusicPlayer objects can be 
  instantiated, and each will use an available timer. On the Arduino Uno, there are two Timers - Timer1 and Timer2 -
  available, so two objects can be instantiated. You can therefore have simultaneous playback of two different music tracks
  using the two timers. The example PlayTunes plays the melody of Do-Re-Mi on one speaker, and the left-hand 
  harmony on a second speaker. It also uses the callback functionality of the MmlMusic library to also 
  display synchronized lyrics on an LCD screen. 

  The musical notation is in MML, and you can refer to the MmlMusic library (which this library extends) for 
  additional information on the format.

  The ProtoStax_MmlMusicPlayer library is inspired by and borrows from the MmlMusicPWM library, the bhagman Tone library 
  (for the parts about keeping track of the various timers), and the NewTone library (for Timer1 settings). 
  Use of this library can affect pins 3, 11, (when Timer2 is used) 9 and 10 (when Timer1 is used). 

  Placeholders are in place to extend this for other Arduino platforms (Mega, ESP) and utilize their Timers (not 
  finished and not tested (duh!))

  We have invested a lot of time and effort into providing this and other open source code. 
  Please support us by purchasing products from ProtoStax!

 
  Written by Sridhar Rajagopal for ProtoStax
  BSD license. All text above must be included in any redistribution
 */

#ifndef _PROTOSTAX_MMLMUSICPLAYER_H
#define _PROTOSTAX_MMLMUSICPLAYER_H

#include "MmlMusic.h"

/*
|| Definitions
*/

class ProtoStax_MmlMusicPlayer : public MmlMusic 
{
  public:
    void begin(uint8_t tonePin);
    void _play(uint16_t frequency, uint32_t duration = 0);
    void _stop();
    void _waitTone(unsigned long length);
    void _executeCommandTim(void);
    
    void playTone(unsigned int frequency, unsigned long length, uint8_t nTrack=0, uint8_t nVolume=0);		// public to allow access from callback

  private:
    void setTimerStuff();
    void setPin();

    static uint8_t _tone_pin_count;
    uint8_t _pin;
    int8_t _timer;
    bool _pinSet;
    const int _tTim2_prescale[7] = { 2, 16, 64, 128, 256, 512, 2048 }; // Prescaler.
    
};

#endif

# ProtoStax_MmlMusicPlayer PlayTunes Example Sketch

This is a example sketch for the ProtoStax Mml Music Player library

![ProtoStax Arduino Nano BLE RGB LED Service Demo](ProtoStax_Arduino_Nano_BLE_RGBLED_Service_Demo.jpg)

   
  It uses
  Arduino Uno,
  [ProtoStax for Arduino](https://www.protostax.com/collections/all/products/protostax-for-arduino),
  [ProtoStax LCD Kit V2](https://www.protostax.com/collections/all/products/protostax-lcd-kit-v2),
  [ProtoStax Kit for Momentary Push Button Switches](https://www.protostax.com/collections/all/products/protostax-kit-for-momentary-push-button-switches)

  Piezo Buzzer PS1240 x2,
  Jumper wires 

For further instructions, see the [Multi-track Music Player](https://create.arduino.cc/projecthub/sridhar-rajagopal/multi-track-music-player-f661ba)
Tutorial at [Arduino Project Hub](https://create.arduino.cc/projecthub/sridhar-rajagopal/multi-track-music-player-f661ba)

  The following pins are used in this example - 
  SCL and SDA pins for I2C communications with the 16x2 LCD I2C module, pins 12 and 13 for the buzzers, and 
  pins 2 and 3 for the push buttons. 

  This uses the ProtoStax_MmlMusicPlayer library, which extends the MmlMusic library to provide autoplay 
  capabilities using the Timers on the Arduino. Multiple ProtoStax_MmlMusicPlayer objects can be 
  instantiated, and each will use an available timer. On the Arduino Uno, there are two Timers - Timer1 and Timer2 -
  available, so two objects can be instantiated. You can have simultaneous autoplay of two different music tracks. 

  In this example, player 1 plays the melody of Do Re Mi, while player2 plays the finger-style (one note at a time) harmony (chords) for
  the song. It also displays the synchronized lyrics of the song on the LCD screen. 

  One of the buttons is used to start the music, while the other button is used to stop the music when playing. 

  The musical notation is in MML, and you can refer to the MmlMusic library (which this library extends) for 
  additional information on the format.

  The ProtoStax_MmlMusicPlayer library is inspired by and borrows from the MmlMusicPWM library, the bhagman Tone library,
  and the NewTone library. Use of this library can affect pins 3, 11, (when Timer2 is used) 9 and 10 (when Timer1 is used).


## Prerequisites

Install [MmlPlayer](https://github.com/maxint-rd/MmlMusic) Library using the Library Manager in your Arduino
IDE


## Installing

This library has been added to the library manager list, so you can install the library directly from the Arduino IDE Library Manager by searching for it and pressing install. Alternately, you can download and install the library directly from GitHub:

Download as a Zip file and install using the Library Manager. You can also clone the GitHub repository to your computer and copy it to your Arduino/libraries location manually. 

```
git clone https://github.com/protostax/ProtoStax_MmlMusicPlayer.git
```


## Usage

See additional instructions and details in Tutorial linked above

Once the library is installed, this example can be opened from the menu from Files->Examples->ProtoStax Mml Music Player -> PlayTunes

Compile the sketch and upload to your Arduino

Instantiation outside of setup():
```
  // include header and initialize class
  #include <ProtoStax_MmlMusicPlayer.h>
  ProtoStax_MmlMusicPlayer player1();
  ProtoStax_MmlMusicPlayer player2(); 
  ```

Initialization within setup():
each call to begin sets up and assigns one available timer for that object to use. The tones are generated on the specified pin (connect a piezo or optionally amplified speaker to specified pin and GND).
```
void setup() {
...
    // optionally set callback functions
    player1.setPlayCallback(MyPlayCallback);
    player1.setToneCallback(MyToneCallback);
    player1.setCompletionCallback(MyCompletionCallback);   
    ...
    player1.begin(TONE_PIN_1);
    player2.begin(TONE_PIN_2);
    ...
}
```


Then to play music, call the play method where you want:
```
music.play("T240 L16 O6 C D E F G");
music2.play("T240 L16 O4 C D E F G");
```

This will play both sequences simultaneously (or near simultaneously (within the human ear's capability to distinguish). 

When playing the rest of the sequence, the isPlaying() method can be used to determine if the sequence is still playing.
```
if(music.isPlaying()
{
    ...
}
```
Callback functions can also be used to signal the start or end of playing the tune, and for each note played. The PlayTunes example uses the callback functions to keep track of the note being played and to determine which line of the lyrics to display, for example, on an LCD screen. The lyric display would thus be synchronized with the playback. See the included examples for more detailed instructions on how to use this library.


## License

Written by Sridhar Rajagopal for ProtoStax. BSD license, all text above must be included in any redistribution

A lot of time and effort has gone into providing this and other code. Please support ProtoStax by purchasing products from us!




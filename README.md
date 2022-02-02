# ProtoStax_MmlMusicPlayer
The ProtoStax_MmlMusicPlayer is an Arduino library to play multiple Music Macro Language sequences asynchronously. It extends the [MmlMusic](https://github.com/maxint-rd/MmlMusic) base library to provide multi-sequence asynchronous playback capabilities using any available Timers on the Arduino. 

This has been tested with the Arduino Uno using both Timer 1 and Timer 2.

### Introduction
ProtoStax_MmlMusicPlayer provides a means to play Music Macro Language sequences asynchronously. Where the Arduino tone() function allows for playing one single note, the ProtoStax_MmlMusicPlayer::play() method can play an entire music score. 

Muliple ProtoStax_MmlMusicPlayer objects can be instantiated, and each can play an MML track asynchronously. The number of instances you can have is constrained by the number of available Timers. On the Arduino Uno, there are two Timers (Timer1 and Timer2) available, so two objects can be instantiated. You can thus have simultaneous playback of two different music tracks using the two timers. On an MCU like the Mega, there are 6 timers available, so you should be able to create 6 different asynchronous playback objects for 6 simultaneous track playback (once the code has been extended to support the Mega). 

The example PlayTunes plays the melody of Do-Re-Mi on one speaker, and the left-hand harmony on the 2nd speaker. It also uses the callback functionality of the MmlMusic library to also display synchronized lyrics on an LCD screen. 

The musical notation is in MML, and you can refer to the MmlMusic library (which this library extends) for additional information on the format.

Placeholders are in place to extend this for other Arduino platforms (Mega, ESP) and utilize their Timers (not finished and not tested (duh!))


### Installation/Usage
The library can be downloaded and installed as an Arduino library using the Sketch|Library menu. Just add the zipfile library and the enclosed examples should appear in the menu automatically. It can also be installed via the Arduino Library Manager. Please ensure that you have also installed the MmlMusic library, which this library depends on. MmlMusic library is not available (yet) on the Arduino Library Manager and so has to be installed manually. 

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



### Supported MML Syntax
Short syntax overview: (Please refer to MmlMusic library documentation for additional information)<br>

Command | Description
------------ | -------------
&nbsp;  Tnnn | Set tempo [32-255]. Examples: T120, T240<br>
&nbsp;  Vnnn | Set volume [0-128]. Note: limited effect on PWM-volume. Examples: V1, T120<br>
&nbsp;  Lnn  | Set default note length [1-64]. Examples: L8, L16<br>
&nbsp;  Mx   | Set timing. Mn=default, Ml=legato, Ms=staccato<br>
&nbsp;  On   | Set octave [0-7]. Examples: O6, O7<br>
&nbsp;  A-G  | Play whole note. Example: C<br>
&nbsp;  Ann-Gnn  | Play note of alternative length [1-64]. Example: C4, A16<br>
&nbsp;  Nnn  | Play frequency [0-96]. Example: N48<br>
&nbsp;  #    | Play sharp note. Example: C#<br>
&nbsp;  &plus;    | Alternative for #<br>
&nbsp;  &minus;   | Play flat note. Example: D-<br>
&nbsp;  R    | Rest. Example:  CDEC r CDEC<br>
&nbsp;  P    | Alternative for R. Example:  CDEC p CDEC<br>
&nbsp;  .    | Longer note. Example: CDEC.&nbsp;<br>
&nbsp;  &gt; | shift octave up.  Example: CDE&gt;CDE.&nbsp;<br>
&nbsp;  &lt; | shift octave down.  Example: CDE&lt;CDE.&nbsp;<br>
&nbsp;  , | play multiple tracks  Example: CDE&lt;CDE.,EDC&lt;ECD.&nbsp; (not applicable here - see below) <br>

The supported MML-commands are a subset that may not completely cover all available music scores.
If notes seem missing, check your score against the syntax above and replace unknown commands by equivalent supported alternatives. The music notation is case-insensitive. Spaces are not required but can be used for readability.

### Features & limitations
- Playing back multiple tracks is supported by the base library, but not by this library. This library implements multi-track playback by utilizing multiple instances, each of which uses a single pulse-stream. The comma-command above therefore is not applicable for this library. The documentation of the comma-command above is maintained for information on the base class.
- This library has been tested with an Arduino Uno.
- There was a bug in the MmlMusic base library that could impact playback of notes and the duration of the delay() function. When ending the play-string with a number (eg. "T120 O4 G16"), the player could read beyond the end of the string, play whatever was next in memory and mess up the timer callback. This bug has been fixed. Please use the latest version of the [MmlMusic base library](https://github.com/maxint-rd/MmlMusic).  See [this issue](https://github.com/maxint-rd/MmlMusic/issues/1) for details.

### Credits
The base library is based on the MusicEngine library by Chris Taylor, ported from mBed to Arduino. It is a follow-up of the [ESP-MusicEngine library](https://github.com/maxint-rd/ESP-MusicEngine).

The ProtoStax_MmlMusicPlayer library is inspired by and borrows from the [MmlMusicPWM](https://github.com/maxint-rd/MmlMusicPWM) library, the [bhagman Tone](https://github.com/bhagman/Tone) library (for the parts about keeping track of the various timers), and the [NewTone](https://bitbucket.org/teckel12/arduino-new-tone/src/master/) library (for Timer1 settings). Use of this library can affect pins 3, 11, (when Timer2 is used) 9 and 10 (when Timer1 is used). 

### Links
- Learn more about Music Macro Language (MML) on wikipedia:<br>
   http://en.wikipedia.org/wiki/Music_Macro_Language<br>
- For downloadable MML music see http://www.archeagemmllibrary.com/<br>
- Extensive MML reference guide (not all commands supported):<br>
http://woolyss.com/chipmusic/chipmusic-mml/ppmck_guide.php<br>

## License

Written by Sridhar Rajagopal for ProtoStax. BSD license, all text above must be included in any redistribution

We have invested a lot of time and effort into providing this and other open source code. Please support us by purchasing products from ProtoStax!


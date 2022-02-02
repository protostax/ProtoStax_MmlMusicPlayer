/*************************************************** 
  ProtoStax Mml Music Player Example

  This is a example sketch for the ProtoStax Mml Music Player library  
   
  It uses
  Arduino Uno,
  ProtoStax for Arduino --> https://www.protostax.com/collections/all/products/protostax-for-arduino,
  ProtoStax LCD Kit V2 --> https://www.protostax.com/collections/all/products/protostax-lcd-kit-v2,
  ProtoStax Kit for Momentary Push Button Switches --> 
     https://www.protostax.com/collections/all/products/protostax-kit-for-momentary-push-button-switches

  Piezo Buzzer PS1240 x2,
  Jumper wires 

  The following pins are used in this example - 
  SCL and SDA pins for I2C communications with the 16x2 LCD I2C module, pins 12 and 13 for the buzzers, and 
  pins 2 and 3 for the push buttons. 

  This uses the ProtoStax_MmlMusicPlayer library, which extends the MmlMusic library to provide autoplay 
  capabilities using the Timers on the Arduino. Muliple ProtoStax_MmlMusicPlayer objects can be 
  instantiated, and each will use an available timer. On the Arduino Uno, there are two Timers - Timer1 and Timer2 -
  available, so two objects can be instantiated. You can have simultaneous autoplay of two different music tracks. 

  In this example, player 1 plays the melody of Do Re Mi, while player2 plays the finger-style (one note at a time) harmony (chords) for
  the song. It also displays the synchronized lyrics of the song on the LCD screen. 

  One of the buttons is used to start the music, while the other button is used to stop the music when playing. 

  The musical notation is in MML, and you can refer to the MmlMusic library (which this library extends) for 
  additional information on the format.

  The ProtoStax_MmlMusicPlayer library is inspired by and borrows from the MmlMusicPWM library, the bhagman Tone library,
  and the NewTone library. Use of this library can affect pins 3, 11, (when Timer2 is used) 9 and 10 (when Timer1 is used). 
 
  Written by Sridhar Rajagopal for ProtoStax
  BSD license. All text above must be included in any redistribution
 */

#include <ProtoStax_MmlMusicPlayer.h>
#include <LiquidCrystal_I2C.h>
#include <JC_Button.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

// pin assignments
const byte
    DN_PIN(3),                      // connect a button switch from this pin to ground
    UP_PIN(2);                      // ditto

Button upBtn(UP_PIN), downBtn(DN_PIN);    // define the buttons

const unsigned long
    REPEAT_FIRST(500),              // ms required before repeating on long press
    REPEAT_INCR(100),               // repeat interval for long press
    LONG_PRESS(1000);

#define TONE_PIN_1 12
#define TONE_PIN_2 13

ProtoStax_MmlMusicPlayer player1;
ProtoStax_MmlMusicPlayer player2;

const char doReMi[] PROGMEM = {"T120 O5"
">C.2< R4"
"C.4 D8 E.4 C8 E4 C4"
"E4 R4 D.4 E8 F8 F8 E8 D8 F.2 R4"
"E.4 F8 G.4 E8 G4 E4 G4 R4 F.4 G8"
"A8 A8 G8 F8 A4 R4 R2 G4 R8 C8 D8 E8 F8 G8"
"A1 A4 R8 D8 E8 F#8 G8 A8"
"B1 B4 R8 E8 F#8 G#8 A8 B8 >C2<"
"R4 B8 B-8 A4 F4 B4 G4"
">C8< B8 A8 G8 F8 E8 D8 C8 D8 C8 R4 R2"
"C.4 D8 E.4 C8 E4 C4"
"E4 R4 D.4 E8 F8 F8 E8 D8 F.2 R4"
"E.4 F8 G.4 E8 G4 E4 G4 R4 F.4 G8"
"A8 A8 G8 F8 A4 R4 R2 G4 R8 C8 D8 E8 F8 G8"
"A1 A4 R8 D8 E8 F#8 G8 A8"
"B1 D4 R8 E8 F#8 G#8 A8 B8 >C2<"
"R4 B8 B-8 A4 F4 B4 G4"
"C8 B8 A8 G8 F8 E8 D8 C8 G4 F4 >C4< R4" };

const char doReMiChords[] PROGMEM = {"T120 O4 "
"R8 C8 D8 E8 F8 G8 A8 B8"
"R4 E4 G4 E4 G4 E4"
"G4 >C4< A4 G4 A4 G4 A4 F4 A4 G4"
"R4 C4 E4 G4 E4 G4 E4 >C4< B4 G4"
"B4 G4 D4 G4 G-4 F4 E4 G4 B8 B-8 A8 G8"
"F4 E4 D4 G4 F#4 A4 D2 G4 F#4"
"E4 D4 G#4 B4 E2 A4 G#4 A4 E4 F4 A4 G4 B4"
">C4< R4 R2 R4 B8 A8 B8 G8 A8 B8"
"R4 E4 G4 E4 G4 E4"
"G4 >C4< A4 G4 A4 G4 A4 F4 A4 G4"
"R4 C4 E4 G4 E4 G4 E4 >C4< B4 G4"
"B4 G4 D4 G4 G-4 F4 E4 G4 B8 B-8 A8 G8"
"F4 E4 D4 G4 F#4 A4 D2 G4 F#4"
"E4 D4 G#4 B4 E2 A4 G#4 A4 E4 F4 A4 G4 B4"
">C4< R4 R2 R4 G4 >C4< C4"};

// Song lyric strings - defined in PROGMEM
// Make sure each string is less than 32 characters (the length of 2x16 characters of the LCD)
// If longer than 16 characters, it will wrap over to second line

const char string1[]  PROGMEM = "Doe, a deer,"; 
const char string2[]  PROGMEM = "a female deer,";
const char string3[]  PROGMEM = "Ray, a drop of";
const char string4[]  PROGMEM = "golden sun,";
const char string5[]  PROGMEM = "Me, a name";
const char string6[]  PROGMEM = "I call myself,";
const char string7[]  PROGMEM = "Far, a long, long";
const char string8[]  PROGMEM = "way to run.";
const char string9[]  PROGMEM = "Sew, a needle";
const char string10[] PROGMEM = "pulling thread.";
const char string11[] PROGMEM = "La, a note to";
const char string12[] PROGMEM = "follow sew.";
const char string13[] PROGMEM = "Tea, a drink";
const char string14[] PROGMEM = "with jam and bread";
const char string15[] PROGMEM = "that will bring us";
const char string16[] PROGMEM = "back to";
const char string17[] PROGMEM = "do.";
const char string18[] PROGMEM = "do ti la sol fa mi re do";

const char string19[] PROGMEM = "...";
const char string20[] PROGMEM = "..";
const char string21[] PROGMEM = ".";

const char string22[] PROGMEM = "Do Re Mi - Sound of Music";


// Song Lyrics
// Fit the lyrics with the notes (for each non-rest note, pick the lyric string to display)
// There are as many items here as the number of notes in the main melody
// As the notes are played, the MyToneCallback function is invoked for each note played
// which will increment the index into this array
// The main loop will display the appropriate lyric as pointed to by the index (called noteNum below)

const char *const noteLyrics[] PROGMEM = {string22,

string1, string1, string1, string2, string2, string2, string2, string3, string3, string3, string3, string4 ,string4, string4, string5, string5, string5, string6, string6, string6, string6, string7, string7, string7, string7, string8, string8, string8, string9, string9, string9, string9, string10, string10, string10, string11, string11, string11, string11, string12, string12, string12, string13, string13, string13, string14, string14, string14, string14, string15, string15, string15, string15, string16, string16,

string17, string17, string17, string17, string17, string17, string17, string17, string17, string17,                           

string1, string1, string1, string2, string2, string2, string2, string3, string3, string3, string3, string4 ,string4, string4, string5, string5, string5, string6, string6, string6, string6, string7, string7, string7, string7, string8, string8, string8, string9, string9, string9, string9, string10, string10, string10, string11, string11, string11, string11, string12, string12, string12, string13, string13, string13, string14, string14, string14, string14, string15, string15, string15, string15, string16, string16,

string18, string18, string18, string18, string18, string18, string18, string18, string19, string20, string21
};

// noteNum represents the index into the array of noteLyrics, and in incremented in the MyToneCallback when a note has been played
// After the song has been played fully or stopped in between, this gets set to -1
volatile int noteNum = -1;

// Lets us know if the song is being played or not. The song can stop playing either:
// 1) when the entire song has been played
// 2) the user presses the upper button to stop the song
volatile bool playing = false;



// toggleLCDmessage is used to toggle between two different messages to display
// This gets invoked by the caller periodically to toggle the messages
void toggleLCDmessage() {
  static bool startMsg = false;
  if (!startMsg) {
    lcd.setCursor(0,0);
    lcd.print("ProtoStax Music");
    lcd.setCursor(0,1);
    lcd.print("     Player     ");
  } else {
    lcd.setCursor(0,0);
    lcd.print("  Press button  ");
    lcd.setCursor(0,1);
    lcd.print("    to start    ");    
  }
  startMsg = !startMsg;

}

void setup() {
  Serial.begin(9600);

  Serial.println("ProtoStax Music Player");

  downBtn.begin();
  upBtn.begin();
  
  lcd.init();
  lcd.backlight();
  toggleLCDmessage();

  // set callback functions
  player1.setPlayCallback(MyPlayCallback);
  player1.setToneCallback(MyToneCallback);
  player1.setCompletionCallback(MyCompletionCallback);   
  
  player1.begin(TONE_PIN_1);
  delay(100);
  player2.begin(TONE_PIN_2);

}

char buffer[64];  // make sure this is large enough for the largest string it must hold

void loop() {
  static unsigned long lastToggleTime = millis();
  
  downBtn.read();
  upBtn.read();

  if (!playing ) {
    if (upBtn.wasReleased()) {
      playing = true;
      lcd.clear();
      player1.play_P(doReMi);
      player2.play_P(doReMiChords);
    } else {
      if (millis() - lastToggleTime > 3000) {
        toggleLCDmessage();
        lastToggleTime = millis();        
      }
    }
  } else {
    if (downBtn.wasReleased()) {
      playing = false;
      noteNum = -1;
      player1.stop();
      player2.stop();
    }
  }
    
  
  if (noteNum >= 0) {
    // The below is for copying out 16 padded bytes for display - for reference
    // We instead use the wrapPrint function to wrap the text over the 2 lines of 
    // the 16x2 display, as required.     
    // memset(buffer, ' ', 16);
    // buffer[16] = '\0';
    // memcpy_P(buffer, (char *)pgm_read_word(&(noteLyrics[noteNum])), strlen_P((char *)pgm_read_word(&(noteLyrics[noteNum]))));    
    strcpy_P(buffer, (char *)pgm_read_word(&(noteLyrics[noteNum])));   
    wrapPrint(buffer);   
  }
}


// This callback gets invoked for each note(tone) being played
// We use this to keep track of noteNum (to figure which lyric to display)
// We also use the opportunity to print some diagnostic information
// We return false to indicate that we have not actually played the tone
// so the MmlMusic object will invoke its child's playTone method to do the job
bool MyToneCallback(unsigned int frequency, unsigned long length, uint8_t nTrack, uint8_t nVolume)
{   // the tone callback is called on three occasions
  // 1. play a tone of certain frequency for specified length or indefinitely (len=0)
  // 2. wait for a certain period while tone or silence is played (freq=0, len=msec)
  // 3. stop playing (freq=0; len=0)
  
  if(frequency==0 && length==0)
  {
    //Serial.println(F("shhht"));
  }
  else
  {
    if(frequency>0)
    {
      Serial.print(nTrack);
      Serial.print(F("t:"));
      Serial.print(frequency);
      Serial.print(F(" "));
      noteNum++;
    }
  }
  return(false);    // return false to indicate callback did not make the tone and wants original playTone to continue
}

// Arduino core has no definition for FPSTR
// on ESP in wstring.h: #define FPSTR(pstr_pointer) (reinterpret_cast<const __FlashStringHelper *>(pstr_pointer))
#if !defined(FPSTR)
#define FPSTR(pstr_pointer) (reinterpret_cast<const __FlashStringHelper *>(pstr_pointer))
#endif
// Unfortunately the definition above won't compile on Arduino IDE 1.6.7. It will compile in IDE 1.8.2
// You can upgrade or remove this code and the use of MyPlayCallback below.

// Callback functions can also be set to indicate start or end of playing the music sequence
// We use the completion callback to indicate we are no longer playing and to reset the lyric counter
void MyPlayCallback(const char* mml, bool fUseFlash)
{ // Called prior to start of plsy. Note mml may point to PROGMEM string, depending on call to play or play_P
  Serial.print(F("Play "));
  //Serial.println(fUseFlash ? FPSTR(mml) : mml); // can't use conditional expression with distinct pointer types
  if(fUseFlash)
    Serial.println(FPSTR(mml));
  else
    Serial.println(mml);
}

void MyCompletionCallback(void)
{ // Called prior when done playing
  Serial.println();
  Serial.println(" Done!");
  playing = false;
  noteNum = -1;
}

char line1[17];
char line2[17];

// This method is used to wrap print the given string over the 2 lines of the 16x2 display,
// as required
// extra padding with space is done to ensure any previous display is cleared
void wrapPrint(char *str) {
  char *tmp = str;
  int len = strlen(str);

  memset(line1, ' ', 16);
  memset(line2, ' ', 16);
  line1[16] = '\0';
  line2[16] = '\0';

  if (strlen(str) <= 16) {
    strncpy(line1, str, len);    
  } else {
    strncpy(line1, str, 16);
    tmp = tmp+16;
    int len2 = strlen(tmp);
    strncpy(line2, tmp, (len2>16?16:len2));
  }
  lcd.setCursor(0,0);
  lcd.print(line1);
  lcd.setCursor(0,1);
  lcd.print(line2);  
}

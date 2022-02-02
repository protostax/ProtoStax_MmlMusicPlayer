/*************************************************** 
  ProtoStax Mml Music Player

  The ProtoStax_MmlMusicPlayer library extends the MmlMusic library to provide autoplay 
  capabilities using all available Timers on the Arduino. 
  This has been tested with the Arduino Uno and uses Timer 1 and Timer 2.

  Muliple ProtoStax_MmlMusicPlayer objects can be 
  instantiated, and each will use an available timer. On the Arduino Uno, there are two Timers - Timer1 and Timer2 -
  available, so two objects can be instantiated. You can have simultaneous autoplay of two different music tracks
  using the two timers. The example PlayTunes plays the melody of Do-Re-Mi on one speaker, and the left-hand 
  harmony on the other speaker. It also uses the callback functionality of the MmlMusic library to also 
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

#include "ProtoStax_MmlMusicPlayer.h"

//#include <Arduino.h>
#if defined (ARDUINO_ARCH_ESP8266)
    #include <cctype>
#endif

// XXX TODO
// #if defined(__AVR_ATmega8__)
// #define TCCR2A TCCR2
// #define TCCR2B TCCR2
// #define COM2A1 COM21
// #define COM2A0 COM20
// #define OCR2A OCR2
// #define TIMSK2 TIMSK
// #define OCIE2A OCIE2
// #define TIMER2_COMPA_vect TIMER2_COMP_vect
// #define TIMSK1 TIMSK
// #endif

#if !defined(__AVR_ATmega8__)
volatile unsigned long timer0_time; // Used to track end note with timer when playing note in the background.
volatile uint8_t *timer0_pin_port;
volatile uint8_t timer0_pin_mask;
volatile uint8_t *timer0_pin_mode;     
volatile ProtoStax_MmlMusicPlayer* __thisMmlMusicPlayer0__;
#endif
volatile unsigned long timer1_time; // Used to track end note with timer when playing note in the background.
volatile uint8_t *timer1_pin_port;
volatile uint8_t timer1_pin_mask;
volatile uint8_t *timer1_pin_mode;     
volatile ProtoStax_MmlMusicPlayer* __thisMmlMusicPlayer1__;

volatile unsigned long timer2_time; // Used to track end note with timer when playing note in the background.
volatile uint8_t *timer2_pin_port;
volatile uint8_t timer2_pin_mask;
volatile uint8_t *timer2_pin_mode;     
volatile ProtoStax_MmlMusicPlayer* __thisMmlMusicPlayer2__;;


#if defined(__AVR_ATmega1280__)
volatile unsigned long timer3_time; // Used to track end note with timer when playing note in the background.
volatile uint8_t *timer3_pin_port;
volatile uint8_t timer3_pin_mask;
volatile uint8_t *timer3_pin_mode;     
volatile ProtoStax_MmlMusicPlayer* __thisMmlMusicPlayer3__;

volatile unsigned long timer4_time; // Used to track end note with timer when playing note in the background.
volatile uint8_t *timer4_pin_port;
volatile uint8_t timer4_pin_mask;
volatile uint8_t *timer4_pin_mode;     
volatile ProtoStax_MmlMusicPlayer* __thisMmlMusicPlayer4__;

volatile unsigned long timer5_time; // Used to track end note with timer when playing note in the background.
volatile uint8_t *timer5_pin_port;
volatile uint8_t timer5_pin_mask;
volatile uint8_t *timer5_pin_mode;     
volatile ProtoStax_MmlMusicPlayer* __thisMmlMusicPlayer5__;

#endif


#if defined(__AVR_ATmega1280__)

#define AVAILABLE_TONE_PINS 6

// Leave timers 1, and zero to last.
const uint8_t PROGMEM tone_pin_to_timer_PGM[] = { 2, 3, 4, 5, 1, 0 };

#elif defined(__AVR_ATmega8__)

#define AVAILABLE_TONE_PINS 2

const uint8_t PROGMEM tone_pin_to_timer_PGM[] = { 2, 1 };

#else

#define AVAILABLE_TONE_PINS 3

// Leave timer 0 to last.
const uint8_t PROGMEM tone_pin_to_timer_PGM[] = { 2, 1, 0 };

#endif


// Initialize our pin count

uint8_t ProtoStax_MmlMusicPlayer::_tone_pin_count = 0;


// Interrupt routines
#if !defined(__AVR_ATmega8__)
#ifdef WIRING
void Tone_Timer0_Interrupt(void)
#else
ISR(TIMER0_COMPA_vect)
#endif
{
  if (!__thisMmlMusicPlayer0__)
  	return;

  if (millis() > timer0_time)
  {
    __thisMmlMusicPlayer0__->_stop(); // Check to see if it's time for the note to end.
    __thisMmlMusicPlayer0__->_executeCommandTim(); // execute the next command 
  }
  else {
    // toggle the pin
    *timer0_pin_port ^= timer0_pin_mask;
  }
}
#endif


#ifdef WIRING
void Tone_Timer1_Interrupt(void)
#else
ISR(TIMER1_COMPA_vect)
#endif
{
  if(!__thisMmlMusicPlayer1__)
  	return;

  if (millis() > timer1_time)
  {
    __thisMmlMusicPlayer1__->_stop(); // Check to see if it's time for the note to end.
    __thisMmlMusicPlayer1__->_executeCommandTim(); // execute the next command 
  }
  else {
    // toggle the pin
    *timer1_pin_port ^= timer1_pin_mask;
  }
}


#ifdef WIRING
void Tone_Timer2_Interrupt(void)
#else
ISR(TIMER2_COMPA_vect)
#endif
{  
   if(!__thisMmlMusicPlayer2__) {
     // Serial.println("Hmm");
     //Serial.flush();
   	return;
   }

   if (millis() > timer2_time) {
     __thisMmlMusicPlayer2__->_stop(); // Check to see if it's time for the note to end.
     __thisMmlMusicPlayer2__->_executeCommandTim(); // execute the next command 
   }
   else {
     // toggle the pin
     *timer2_pin_port ^= timer2_pin_mask;
   }
}



#if defined(__AVR_ATmega1280__)

#ifdef WIRING
void Tone_Timer3_Interrupt(void)
#else
ISR(TIMER3_COMPA_vect)
#endif
{
  if(!__thisMmlMusicPlayer3__)
    return;

  if (millis() > timer3_time)
  {
    __thisMmlMusicPlayer3__->_stop(); // Check to see if it's time for the note to end.
    __thisMmlMusicPlayer3__->_executeCommandTim(); // execute the next command 
  }
  else {
    // toggle the pin
    *timer3_pin_port ^= timer3_pin_mask;
  }  
}

#ifdef WIRING
void Tone_Timer4_Interrupt(void)
#else
ISR(TIMER4_COMPA_vect)
#endif
{
  if(!__thisMmlMusicPlayer4__)
  	return;

  if (millis() > timer4_time)
  {
    __thisMmlMusicPlayer4__->_stop(); // Check to see if it's time for the note to end.
    __thisMmlMusicPlayer4__->_executeCommandTim(); // execute the next command 
  }
  else {
    // toggle the pin
    *timer4_pin_port ^= timer4_pin_mask;

  }  

}

#ifdef WIRING
void Tone_Timer5_Interrupt(void)
#else
ISR(TIMER5_COMPA_vect)
#endif
{
  if(!__thisMmlMusicPlayer5__)
  	return;

  if (millis() > timer5_time)
  {
    __thisMmlMusicPlayer5__->stop(); // Check to see if it's time for the note to end.
    __thisMmlMusicPlayer5__->_executeCommandTim(); // execute the next command 
  }
  else {
    // toggle the pin
    *timer5_pin_port ^= timer5_pin_mask;

  }   
}

#endif

// XXX TODO - Do we need this? 
void ProtoStax_MmlMusicPlayer::setTimerStuff()
{
    // Set timer specific stuff
    // All timers in CTC mode
    // 8 bit timers will require changing prescalar values,
    // whereas 16 bit timers are set to either ck/1 or ck/64 prescalar
    switch (_timer)
    {
#if !defined(__AVR_ATmega8__)
      case 0:
#ifdef WIRING
        Timer0.attachInterrupt(INTERRUPT_COMPARE_MATCH_A, Tone_Timer0_Interrupt);
#endif
        break;
#endif

      case 1:
#ifdef WIRING
        Timer1.attachInterrupt(INTERRUPT_COMPARE_MATCH_A, Tone_Timer1_Interrupt);
#endif
        break;
      case 2:
#ifdef WIRING
        Timer2.attachInterrupt(INTERRUPT_COMPARE_MATCH_A, Tone_Timer2_Interrupt);
#endif
      break;

#if defined(__AVR_ATmega1280__)
      case 3:
#ifdef WIRING
        Timer3.attachInterrupt(INTERRUPT_COMPARE_MATCH_A, Tone_Timer3_Interrupt);
#endif
        break;
      case 4:
#ifdef WIRING
        Timer4.attachInterrupt(INTERRUPT_COMPARE_MATCH_A, Tone_Timer4_Interrupt);
#endif
        break;
      case 5:
#ifdef WIRING
        Timer5.attachInterrupt(INTERRUPT_COMPARE_MATCH_A, Tone_Timer5_Interrupt);
#endif
        break;
#endif
    }

}


void ProtoStax_MmlMusicPlayer::begin(uint8_t tonePin)
{
  if (_tone_pin_count < AVAILABLE_TONE_PINS)
  {
    _pin = tonePin;
    _timer = pgm_read_byte(tone_pin_to_timer_PGM + _tone_pin_count);
    _tone_pin_count++;
    _pinSet = false;

    pinMode(_pin, OUTPUT);
    this->noTone();

    // setTimerStuff();

  }
  else
  {
    // disabled
    _timer = -1;
  }
}

void ProtoStax_MmlMusicPlayer::setPin()
{
    // This gets the port register and bitmap for the pin and sets the pin to output mode.
    switch (_timer)
    {
#if !defined(__AVR_ATmega8__)
      case 0:
        timer0_pin_port = portOutputRegister(digitalPinToPort(_pin));
        timer0_pin_mask = digitalPinToBitMask(_pin);
        timer0_pin_mode = (uint8_t *) portModeRegister(digitalPinToPort(_pin));
        *timer0_pin_mode |= timer0_pin_mask;
        break;
#endif

      case 1:
        timer1_pin_port = portOutputRegister(digitalPinToPort(_pin));
        timer1_pin_mask = digitalPinToBitMask(_pin);
        timer1_pin_mode = (uint8_t *) portModeRegister(digitalPinToPort(_pin));
        *timer1_pin_mode |= timer1_pin_mask;        
        break;
      case 2:
        timer2_pin_port = portOutputRegister(digitalPinToPort(_pin));
        timer2_pin_mask = digitalPinToBitMask(_pin);
        timer2_pin_mode = (uint8_t *) portModeRegister(digitalPinToPort(_pin));
        *timer2_pin_mode |= timer2_pin_mask; 
        break;
      
#if defined(__AVR_ATmega1280__)
      case 3:
        timer3_pin_port = portOutputRegister(digitalPinToPort(_pin));
        timer3_pin_mask = digitalPinToBitMask(_pin);
        timer3_pin_mode = (uint8_t *) portModeRegister(digitalPinToPort(_pin));
        *timer3_pin_mode |= timer3_pin_mask; 
        break;
      case 4:
        timer4_pin_port = portOutputRegister(digitalPinToPort(_pin));
        timer4_pin_mask = digitalPinToBitMask(_pin);
        timer4_pin_mode = (uint8_t *) portModeRegister(digitalPinToPort(_pin));
        *timer4_pin_mode |= timer4_pin_mask; 
        break;
      case 5:
        timer5_pin_port = portOutputRegister(digitalPinToPort(_pin));
        timer5_pin_mask = digitalPinToBitMask(_pin);
        timer5_pin_mode = (uint8_t *) portModeRegister(digitalPinToPort(_pin));
        *timer5_pin_mode |= timer5_pin_mask; 
        break;
#endif
    }

}



// frequency (in hertz) and duration (in milliseconds).

void ProtoStax_MmlMusicPlayer::playTone(unsigned int frequency, unsigned long length, uint8_t nTrack, uint8_t nVolume)
{

  // Serial.print("playTone with _timer:frequency:length="); Serial.print(_timer); Serial.print(":"); Serial.print(frequency); Serial.print(":"); Serial.println(length);
  // Serial.flush();

  
  if(frequency==0 && length==0) {  	
    _stop();
  }  else if(frequency>0) {
    _play(frequency, length);
  } else {
    _waitTone(length);
  }
}

void ProtoStax_MmlMusicPlayer::_play(uint16_t frequency, unsigned long length)
{
  // Serial.print("_play with _timer:frequency:length="); Serial.print(_timer); Serial.print(":"); Serial.print(frequency); Serial.print(":"); Serial.println(length);
  // Serial.flush();

  uint8_t prescalarbits = 0b001;
  unsigned long waitTime = 0;
  uint32_t ocr = 0;
  long top;
  uint8_t prescaler;  

  if (_timer >= 0)
  {    
    // if we are using an 8 bit timer, scan through prescalars to find the best fit
    if (_timer == 0 || _timer == 2)
    {
      for (prescaler = 1; prescaler < 8; prescaler++) { // Find the appropriate prescaler
        top = F_CPU / (long) frequency / (long) _tTim2_prescale[prescaler - 1] - 1; // Calculate the top.
        if (top < 256) break; // Fits, break out of for loop.
      }
      if (top > 255) { _stop(); return; } // Frequency is out of range, turn off sound and return.     
    }
    else
    {
      prescaler = _BV(CS10);                 // Try using prescaler 1 first.
      top = F_CPU / (long)frequency / 4 - 1; // Calculate the top.
      if (top > 65535) {                             // If not in the range for prescaler 1, use prescaler 256 (61 Hz and lower @ 16 MHz).
        prescaler = _BV(CS12);                       // Set the 256 prescaler bit.
        top = top / 256 - 1;                         // Calculate the top using prescaler 256.
      }

    }
    
    // Calculate the wait time
    waitTime = (length > 0) ? (millis() + length - 1) : 0xFFFFFFFF;

    if (!_pinSet) {
      // Set the pinMode as OUTPUT - Done in begin()
      //pinMode(_pin, OUTPUT);
      setPin();
    }    

    // Set the OCR for the given timer,
    // set the toggle count,
    // then turn on the interrupts
    switch (_timer)
    {

#if !defined(__AVR_ATmega8__)
      case 0:
        // XXX TODO
        __thisMmlMusicPlayer0__ = this;
        timer0_time = waitTime;
        OCR0A = top;
        // XXX
        TCCR0B = _BV(WGM02) | prescaler;
        TCCR0A  = _BV(WGM00) | _BV(WGM01); // Fast PWM and normal port operation, OC2A/OC2B disconnected.
        TIMSK0 |= _BV(OCIE0A);             // Activate the timer interrupt.
        break;
#endif

      case 1:
        __thisMmlMusicPlayer1__ = this;
        timer1_time = waitTime;        
        ICR1 = top;
        if (TCNT1 > top) TCNT1 = top; // Counter over the top, put within range.
        TCCR1B  = _BV(WGM13)  | prescaler; // Set PWM, phase and frequency corrected (ICR1) and prescaler.
        TCCR1A  = _BV(COM1B0);
        TIMSK1 |= _BV(OCIE1A); // Activate the timer interrupt.            
        break;
      case 2:        
        __thisMmlMusicPlayer2__ = this;
        timer2_time = waitTime;        
        OCR2A   = top; // Set the top.
        if (TCNT2 > top) TCNT2 = top; // Counter over the top, put within range.
        TCCR2B  = _BV(WGM22) | prescaler;  // Set Fast PWM and prescaler.
        TCCR2A  = _BV(WGM20) | _BV(WGM21); // Fast PWM and normal port operation, OC2A/OC2B disconnected.
        TIMSK2 |= _BV(OCIE2A); // Activate the timer interrupt.
        break;

#if defined(__AVR_ATmega1280__)
      case 3:
        // XXX TODO
        __thisMmlMusicPlayer3__ = this;
        timer3_time = waitTime;        
        OCR3A = top;
        TCCR3B = (TCCR3B & 0b11111000) | prescalarbits;
        
        bitWrite(TIMSK3, OCIE3A, 1);
        break;
      case 4:
        // XXX TODO
        __thisMmlMusicPlayer4__ = this;
        timer4_time = waitTime;        
        OCR4A = top;
        TCCR4B = (TCCR4B & 0b11111000) | prescalarbits;
        
        bitWrite(TIMSK4, OCIE4A, 1);
        break;
      case 5:
        // XXX TODO
        __thisMmlMusicPlayer5__ = this;
        timer5_time = waitTime;        
        OCR5A = top;
        TCCR5B = (TCCR5B & 0b11111000) | prescalarbits;
        
        bitWrite(TIMSK5, OCIE5A, 1);
        break;
#endif

    }
  }
}


void ProtoStax_MmlMusicPlayer::_stop()
{

  // Serial.print("_stop with _timer="); Serial.println(_timer);
  // Serial.flush();

  // Stop playing a tone and restore the PWM settings
  // XXX this only works for Timers 1 and 2
  switch (_timer)
  {
#if !defined(__AVR_ATmega8__)
    case 0:
      // XXX TODO
      TIMSK0 &= ~(1 << OCIE0A);
      *timer0_pin_port &= ~timer0_pin_mask;
      break;
#endif
    case 1:
      
      TIMSK1 &= ~_BV(OCIE1A);     // Remove the timer interrupt.
      TCCR1B  = _BV(CS11);        // Default clock prescaler of 8.
      TCCR1A  = _BV(WGM10);       // Set to defaults so PWM can work like normal (PWM, phase corrected, 8bit).
      *timer1_pin_port &= ~timer1_pin_mask; // Set pin to INPUT
      timer1_pin_mask = 0; // Stops any stray sounds from ISR
      break;
    case 2:
      TIMSK2 &= ~_BV(OCIE2A);     // Remove the timer interrupt.
      TCCR2B  = _BV(CS22);        // Default clock prescaler of 64.
      TCCR2A  = _BV(WGM20);       // Set to defaults so PWM can work like normal (PWM, phase corrected, 8bit).
      *timer2_pin_port &= ~timer2_pin_mask;  // Set pin to INPUT
      timer2_pin_mask = 0;  // Stops any stray sounds from ISR
      break;

#if defined(__AVR_ATmega1280__)
    case 3:
      // XXX TODO
      TIMSK3 &= ~(1 << OCIE3A);
      *timer3_pin_port &= ~timer3_pin_mask;
      timer3_pin_mask = 0;
      break;
    case 4:
      // XXX TODO
      TIMSK4 &= ~(1 << OCIE4A);
      *timer4_pin_port &= ~timer4_pin_mask;
      timer4_pin_mask = 0;
      break;
    case 5:
      // XXX TODO
      TIMSK5 &= ~(1 << OCIE5A);
      *timer5_pin_port &= ~timer5_pin_mask;
      timer5_pin_mask = 0;      
      break;
#endif
  }
  _pinSet = false;

  // digitalWrite(_pin, 0);
  
}


void ProtoStax_MmlMusicPlayer::_executeCommandTim(void)
{
  // Serial.println("_executeCommandTim ");
  // Serial.flush();

  
  continuePlaying();
}

void ProtoStax_MmlMusicPlayer::_waitTone(unsigned long length)
{   // set endtime of tone (or no tone) playing to allow callback at end of tone

  // Serial.print("_waitTone with _timer:duration="); Serial.print(_timer); Serial.print(":"); Serial.println(length);
  // Serial.flush();
  
  
  
    unsigned long waitTime = millis() + length - 1;
  
    switch (_timer)
    {

#if !defined(__AVR_ATmega8__)
      case 0:
        // XXX TODO
        timer0_time = waitTime;        
        __thisMmlMusicPlayer0__ = this;
        bitWrite(TIMSK0, OCIE0A, 1);
        break;
#endif

      case 1:
        timer1_time = waitTime;                
        __thisMmlMusicPlayer1__ = this;        
        TIMSK1 |= _BV(OCIE1A); // Activate the timer interrupt.
        break;
      case 2:

        timer2_time = waitTime;                
        __thisMmlMusicPlayer2__ = this;
        TIMSK2 |= _BV(OCIE2A);             // Activate the timer interrupt.
        break;

#if defined(__AVR_ATmega1280__)
      case 3:
        // XXX TODO
        timer3_time = waitTime;                
        __thisMmlMusicPlayer3__ = this;        
        bitWrite(TIMSK3, OCIE3A, 1);
        break;
      case 4:
        // XXX TODO
        timer4_time = waitTime;                
        __thisMmlMusicPlayer4__ = this;        
        bitWrite(TIMSK4, OCIE4A, 1);
        break;
      case 5:
        // XXX TODO
        timer5_time = waitTime;                
        __thisMmlMusicPlayer5__ = this;        
        bitWrite(TIMSK5, OCIE5A, 1);
        break;
#endif

    }
}





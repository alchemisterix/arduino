
/*** 
 *  0.1 add filename generation for mp3
 *  0.2 add mp3 support
 *  0.3 moved busy pin for pcb layout
***/

#include "Tone.h"
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>


Tone speaker;
SoftwareSerial mySerial(8, 9); // RX, TX

// these pin assignments are for Fast & Easy Instructables configuration using modules noted above


const int MP3_BUSY_PIN = 10;
const int SPEAKERPIN = 10;
const int button[] = {14, 15, 16, 17}; //The four button input pins
const int ledpin[] = {2, 3, 4, 5};     //thee 4 LED pins
const int PSUDOGND3 = 11;  // used as -neg for buzzer
const int ONBRD_LED = 13;


int starttune[] = {NOTE_C4, NOTE_F4, NOTE_C4, NOTE_F4, NOTE_C4, NOTE_F4, NOTE_C4, NOTE_F4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_F4, NOTE_G4};
int duration2[] = {100, 200, 100, 200, 100, 400, 100, 100, 100, 100, 200, 100, 500};
int cheer[] = {NOTE_C4, NOTE_C4, NOTE_G4, NOTE_C5, NOTE_G4, NOTE_C5};
int btnNote[] = {NOTE_G3, NOTE_A3, NOTE_B3, NOTE_C4};
int duration[] = {100, 100, 100, 300, 100, 300};

int turn = 0;  // turn counter
int buttonstate = 0;  // button state checker
int mp3state;
int randomArray[100]; //Intentionally long to store up to 100 inputs (doubtful anyone will get this far)
int inputArray[100];



// ===================================================
void setup()
{

  pinMode(7, INPUT);  // button pins are inputs
  int note;
  Serial.begin(9600);
  
  mySerial.begin (9600);
  mp3_set_serial (mySerial);  //set softwareSerial for DFPlayer-mini mp3 module 
  delay(1);  //wait 1ms for mp3 module to set volume
  mp3_reset();
  delay (1000);
  mp3_set_volume (10);
  delay (400);
  mp3_play (99);
  delay (100);
  while (digitalRead(MP3_BUSY_PIN) == LOW) {};
  
  speaker.begin(SPEAKERPIN);

  for (int x = 0; x < 4; x++) // LED pins are outputs
  {
    pinMode(ledpin[x], OUTPUT);
    pinMode(button[x], INPUT);  // button pins are inputs
    digitalWrite(button[x], HIGH);  // enable internal pullup; buttons start in high position; logic reversed
  }

    pinMode(PSUDOGND3, OUTPUT);
    digitalWrite(PSUDOGND3, LOW);

  randomSeed(analogRead(0)); //Added to generate "more randomness" with the randomArray for the output function
  for (int thisNote = 0; thisNote < 13; thisNote ++) {
    // play the next note:
    note = starttune[thisNote];
    speaker.play(note);
    if (note == NOTE_C4)      digitalWrite(ledpin[0], HIGH);
    else if (note == NOTE_F4) digitalWrite(ledpin[1], HIGH);
    else if (note == NOTE_G4) digitalWrite(ledpin[2], HIGH);
    else if (note == NOTE_E4) digitalWrite(ledpin[3], HIGH);

    // hold the note:
    delay(duration2[thisNote]);
    // stop for the next note:
    speaker.stop();
    allLEDs(LOW);
    delay(25);
  }
  delay(1000);
}

// ===================================================
void loop()
{
  for (int level = 0; level <= 99; level++)
  {
    allLEDs(HIGH);

    for (int thisNote = 0; thisNote < 6; thisNote ++) {
      // play the next note:
      speaker.play(cheer[thisNote]);
      // hold the note:
      delay(duration[thisNote]);
      // stop for the next note:
      speaker.stop();
      delay(25);
    }

    allLEDs(LOW);
    delay(1000);

    Serial.print("\n Turn: ");   // Some serial output to follow along
    Serial.print(turn);
    // fill up the array to be matched by the player
    randomArray[turn] = random(1, 5); //Assigning a random number (1-4) to the randomArray[turn count]
    Serial.print("\n  Rand#: ");
    Serial.println(randomArray[turn]);
    
    mp3_play (5);                                 //Say "your password"
    delay (100);
    while (digitalRead(MP3_BUSY_PIN) == LOW) {};

    for (int x = 0; x <= turn; x++)
    {
      Serial.print(randomArray[x]);
      byte btn = randomArray[x];  // logical button/led # (1-4)
      digitalWrite(ledpin[btn - 1], HIGH);
      speaker.play(btnNote[btn - 1], 100);
      //text = randomArray[x];
      
      mp3_play (randomArray[x]);    
      delay (100);                         //Say "your password"
      while (digitalRead(MP3_BUSY_PIN) == LOW) {};

      
//      Serial.print(text);
      delay(400); 
      digitalWrite(ledpin[btn - 1], LOW);
      delay(100);
    }
    input();
  }
}

// ===================================================
//            local procedures

void input() { //Function for allowing user input and checking input against the generated array
  bool btnPushed;

  for (int x = 0; x <= turn; x++)
  { // for each one in the current sequence wait for a button to be pushed 
    // if it is the correct one we keep looping through the sequence

    
    btnPushed = false;
    while (! btnPushed) {
      for (int y = 0; y < 4; y++)
      {
        buttonstate = digitalRead(button[y]);
        byte btn = y + 1; // logical button # (1-4)

        if (buttonstate == LOW)  //Checking for button push TBD: use BTN_PRESS
        {
          btnPushed = true;
          digitalWrite(ledpin[y], HIGH);
          speaker.play(btnNote[btn - 1], 100);
          delay(100);  // insures minimum LED illumination
          inputArray[x] = btn;
          Serial.print("  btn pushed: ");
          Serial.print(btn);
// was a poor way to allow for button release:         delay(250);
          wait_BtnRelease();  // much better this way
          digitalWrite(ledpin[y], LOW);
          if (inputArray[x] != randomArray[x]) { //Checks value input by user and checks it against
            fail();                              //the value in the same spot on the generated array
            //The fail function is called if it does not match
            exit;
          }
        }
      }
    }  // end while
  }
  if (turn > 0)
    {
    mp3_play (97);                        //Play succes sound
    delay(500);
   // while (digitalRead(MP3_BUSY_PIN) == LOW) {};
    }
  turn++; //Increments the turn count, also the last action before starting the output function over again
}

// ------------------------------------------------------
void fail() { //Function used if the player fails to match the sequence
 mp3_play (98);   
  for (int y = 0; y <= 2; y++)
  { //Flashes lights for failure
    allLEDs(HIGH);
    speaker.play(NOTE_G3, 300);
    delay(200);
    allLEDs(LOW);
    speaker.play(NOTE_C3, 300);
    delay(200);
  }
 
  delay(500);

  
  turn = -1; //Resets turn value so the game starts over without need for a reset button
}

// ------------------------------------------------------
void allLEDs(byte state) {
  digitalWrite(ledpin[0], state);
  digitalWrite(ledpin[1], state);
  digitalWrite(ledpin[2], state);
  digitalWrite(ledpin[3], state);
}

void wait_BtnRelease() {
  bool btnStillDown;
  int debounce=0; // need depends on the button used. mine caused some double trips
    while (debounce<2) {
      delay(5);
      btnStillDown = false;
      for (int y = 0; y < 4; y++)
      {
        buttonstate = digitalRead(button[y]);
        if (buttonstate == LOW)  //Checking for button push
          btnStillDown = true;
      }
      if (btnStillDown) debounce=0;
      else debounce++;
    }
}


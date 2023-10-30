/************************************************************************
 * TinyCircuits Buzzer Wireling Example
 * This program plays a little melody on the buzzer based on notes found 
 * in pitches.h.
 *
 * Based heavily on the Arduino toneMelody sketch found here: 
 * https://www.arduino.cc/en/Tutorial/toneMelody
 *
 * Hardware by: TinyCircuits
 * Written by: Zachary Lee for TinyCircuits
 * 
 * Initialized: 7/31/19 
 * Last updated: 12/04/19
 ************************************************************************/

#include <Wireling.h> // Interface with Wirelings 
#include "pitches.h"
const int pin = A0; // A0 for port 0 on Wireling Adapter, A1 for port 1, etc

// Notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// Note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

void setup() {
  // Power & initialize Wireling
  Wireling.begin();

  // Iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // To calculate the note duration, take one second divided by the note type.
    // Ex. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(pin, melody[thisNote], noteDuration);

    // To distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // Stop the tone playing:
    noTone(8);
  }
}

void loop() {
  // Put tones here if you want them to loop repeatedly
}


// #include <Wireling.h> // Interface with Wirelings
// #include "pitches.h"
// const int pin = A0; // A0 for port 0 on Wireling Adapter, A1 for port 1, etc

// // Notes in the melody:
// int melody[] = {
//   NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
// };

// // Note durations: 4 = quarter note, 8 = eighth note, etc.:
// int noteDurations[] = {
//   4, 8, 8, 4, 4, 4, 4, 4
// };

// void setup() {
//   Wireling.begin(); // Power & initialize Wireling
// }

// void loop() {
//   // Iterate over the notes of the melody and play them in a loop:
//   for (int thisNote = 0; thisNote < 8; thisNote++) {
//     int noteDuration = 1000 / noteDurations[thisNote];
//     tone(pin, melody[thisNote], noteDuration);
//     int pauseBetweenNotes = noteDuration * 1.30;
//     delay(pauseBetweenNotes);
//     noTone(pin); // Stop the tone playing
//   }
// }

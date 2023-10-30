#include <Wireling.h> // Interface with Wirelings
#include "pitches.h"
const int pin = A0; // A0 for port 0 on Wireling Adapter, A1 for port 1, etc

// Notes in the melody for a longer song:
int melody[] = {
  NOTE_E4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_D4,
  NOTE_C4, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_E4,
  NOTE_E4, NOTE_F4, NOTE_G4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_C4,
  NOTE_C4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_C4
};

// Note durations for the longer song: 4 = quarter note, 8 = eighth note, etc.
int noteDurations[] = {
  4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4
};

void setup() {
  Wireling.begin(); // Power & initialize Wireling
}

void loop() {
  // Iterate over the notes of the melody and play them in a loop:
  for (int thisNote = 0; thisNote < 30; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(pin, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(pin); // Stop the tone playing
  }
}

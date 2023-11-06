
#include <Wireling.h>
#include "pitches.h"

const int pin = A0; // A0 for port 0 on Wireling Adapter, A1 for port 1, etc.

// Notes in the new melody for a different notification:
int melody[] = {
  NOTE_D5, NOTE_E5, NOTE_G5, NOTE_D6
};

// Note durations for the new melody: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 4, 2
};

void setup() {
  Wireling.begin();
  playNotificationMelody();
}

void loop() {
  // Put other code here or leave it empty
}

void playNotificationMelody() {
  for (int thisNote = 0; thisNote < 4; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(pin, melody[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(pin); // Use 'pin' instead of 8 to stop the tone.
  }
}

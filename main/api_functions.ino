
/* Audio Libraries */
#include <SD.h>
#include <AudioZero.h>

// =========================================================================
// ||                          FUNCTION - AUDIO                            || 
// ========================================================================= 
void getAudio(void) {
  // open wave file from sdcard
  File myFile = SD.open("COA10.wav");
  // 44100kHz stereo => 88200 sample rate

  if (!myFile) {
    // if the file didn't open, print an error and stop
    SerialMonitorInterface.println("error opening COA10.wav");
    return;
  }

  AudioZero.begin(2*44100);
  SerialMonitorInterface.print("Playing");

  // until the file is not finished  
  AudioZero.play(myFile);
  AudioZero.end();
  
  SerialMonitorInterface.println("End of file. Thank you for listening!");
}

// =========================================================================
// ||                          FUNCTION - PULSE AND BUZZER                 || 
// ========================================================================= 
void getPulseAndBuzzer(void) {
  // enter code here
}

// =========================================================================
// ||                          FUNCTION - Google Calendar                 || 
// ========================================================================= 
void getGoogleCalendar(void) {
  // enter code here
}

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

  // 44100kHz stereo => 88200 sample rate
  AudioZero.begin(2*44100);
  SerialMonitorInterface.print("Playing");

  // until the file is not finished  
  AudioZero.play(myFile);
  AudioZero.end();

  SerialMonitorInterface.println("End of file. Thank you for listening!");
}
// =========================================================================
// ||                      FUNCTION - RTC TIME                            || 
// ========================================================================= 
void displayTime() {    
                  
  for (int i = 1; i < 15; i++) // Display for 15*1000 milliseconds (15 seconds), update display each second
  {

    display.setFont(liberationSansNarrow_12ptFontInfo);  
    months = rtc.getMonth();
    days = rtc.getDay();
    years = rtc.getYear();
    hours = rtc.getHours();
    minutes = rtc.getMinutes();
    seconds = rtc.getSeconds();

    display.setCursor(0,8); //Set the cursor where you want to start printing the date
    if(months < 10) display.print(0); //print a leading 0 if hour value is less than 0
    display.print(months);
    display.print("/");
    days = rtc.getDay();
    if (days < 10) display.print(0);
    display.print(days); 
    // display.print("/");
    // display.print(years);

    display.setCursor(0,25); //Set the cursor where you want to start printing the date  
    setTime(hours,minutes,seconds,days,months,2000 +  years);    //values in the order hr,min,sec,day,month,year
    char wkday[16];
    strcpy(wkday, dayStr(weekday()));
    wkday[3] = ' '; wkday[4] = '\0';
    display.print(wkday); // To keep the text compact, only print the first 3 letters.

    display.setFont(liberationSansNarrow_12ptFontInfo);   //Set the font type

    // display time in HH:MM:SS 24 hour format
    display.setCursor(0,45); //Set the cursor where you want to start printing the time
    if(hours < 10) display.print(0); //print a leading 0 if hour value is less than 0
    display.print(hours);
    display.print(":");
    if(minutes < 10) display.print(0); //print a leading 0 if minute value is less than 0
    display.print(minutes);

   
    delay(1000); //display for 1 seconds
  }
}
// =========================================================================
// ||                          FUNCTION - PULSE AND BUZZER                 || 
// ========================================================================= 
void getPulseAndBuzzer(void) {

  // const int pin = A0; // A0 for port 0 on Wireling Adapter, A1 for port 1, etc.

  // // Notes in the melody for a longer song:
  // int melody[] = {
  //   NOTE_E4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_D4,
  //   NOTE_C4, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_E4,
  //   NOTE_E4, NOTE_F4, NOTE_G4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_C4,
  //   NOTE_C4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_C4
  // };

  // // Note durations for the longer song: 4 = quarter note, 8 = eighth note, etc.
  // int noteDurations[] = {
  //   4, 4, 4, 4, 4, 4, 4, 4,
  //   4, 4, 4, 4, 4, 4, 4, 4,
  //   4, 4, 4, 4, 4, 4, 4, 4,
  //   4, 4, 4, 4, 4, 4
  // };

  //  // Iterate over the notes of the melody and play them in a loop:
  // for (int thisNote = 0; thisNote < 30; thisNote++) {
  //   int noteDuration = 1000 / noteDurations[thisNote];
  //   tone(pin, melody[thisNote], noteDuration);
  //   int pauseBetweenNotes = noteDuration * 1.30;
  //   delay(pauseBetweenNotes);
  //   noTone(pin); // Stop the tone playing
  // }

  SerialMonitorInterface.print("i am in oxi");

  Wireling.selectPort(pulseSensorPort);
  if (pulseSensor.update()) {
    if (pulseSensor.pulseValid()) {
      SerialMonitorInterface.print(pulseSensor.temperature());
      SerialMonitorInterface.print("\t");
      SerialMonitorInterface.print(pulseSensor.BPM());
      SerialMonitorInterface.print("\t");
      SerialMonitorInterface.print(pulseSensor.oxygen());
      SerialMonitorInterface.print("\t");
      SerialMonitorInterface.println(pulseSensor.cardiogram() * 10.0);
    } else {
      SerialMonitorInterface.print(pulseSensor.temperature());
      SerialMonitorInterface.print("\t");
      SerialMonitorInterface.println("-\t-\t-");
    }
  }
  delay(20);//Polling the sensor too often can cause extra noise. The sensor can buffer about 300ms of data with the default settings.


}

// =========================================================================
// ||                          FUNCTION - Google Calendar                 || 
// ========================================================================= 
void getGoogleCalendar(void) {
  // enter code here
}
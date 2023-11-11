
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
  // enter code here
}

// =========================================================================
// ||                          FUNCTION - Google Calendar                 || 
// ========================================================================= 
void getGoogleCalendar(void) {
  // enter code here
}
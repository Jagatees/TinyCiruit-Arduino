// =========================================================================
// ||                          TOOLS - DISPLAY                            ||
// =========================================================================
void printSelected(uint8_t p1, uint8_t p2) {
  if (p1 == p2) {
    display.print(F("--> "));

  } else {
    display.print(F("   "));
  }
}

void clearScreen(void) {
  //for (uint8_t i = 0; i < 100; i++) { display.print(); }
  display.clearWindow(0, 0, 96, 64);
}

void printDivider(void) {
  for (uint8_t i = 0; i < 40; i++) { display.print("-"); }
}

void printDateTime(void) {
  display.clearWindow(20, 45, 50, 16);
  //display.setFont(liberationSansNarrow_12ptFontInfo);   //Set the font type

  while (true) {
    months = rtc.getMonth();
    days = rtc.getDay();
    years = rtc.getYear();
    hours = rtc.getHours();
    minutes = rtc.getMinutes();
    seconds = rtc.getSeconds();

    // Print date in US format MM:DD:YY (Switch the order in which day, month, year that you like to use)
    display.setCursor(15, 8);  //Set the cursor where you want to start printing the date
    if (months < 10) { display.print(0); }
    display.print(months);
    display.print("/");
    if (days < 10) { display.print(0); }
    display.print(days);
    display.print("/");
    display.print(years);

    display.setCursor(30, 25);  //Set the cursor where you want to start printing the date
    setTime(hours, minutes, seconds, days, months, 2000 + years);
    char wkday[16];
    strcpy(wkday, dayStr(weekday()));
    wkday[3] = ' ';
    wkday[4] = '\0';
    display.print(wkday);

    //display.setFont(liberationSansNarrow_16ptFontInfo);   //Set the font type

    // display time in HH:MM:SS 24 hour format
    display.setCursor(20, 45);          //Set the cursor where you want to start printing the time
    if (hour() < 10) display.print(0);  //print a leading 0 if hour value is less than 0
    display.print(hours);
    display.print(":");
    if (minute() < 10) display.print(0);  //print a leading 0 if minute value is less than 0
    display.print(minutes);
    display.print(":");
    if (second() < 10) display.print(0);  //print a leading 0 if seconds value is less than 0
    display.print(seconds);
    display.print(" ");  //just a empty space after the seconds
    //delay(1000); //delay 60 second
  }

  
}

void printBtnArrows(void) {
  leftArrow(0, 15 + 2);
  rightArrow(0, 45 + 5);

  upArrow(90, 15 + 2);
  downArrow(90, 45 + 4);
}


// =========================================================================
// ||                          TOOLS - BUTTON PRESSING                     ||
// =========================================================================

boolean btnIsDown(int btn) {
  //return digitalRead(btn) == LOW && digitalRead(btn) == LOW;
  return (display.getButtons() & btn);
}

boolean btnIsUp(int btn) {
  //return digitalRead(btn) == HIGH && digitalRead(btn) == HIGH;
  return !(display.getButtons() & btn);
}
// =========================================================================
// ||                                 ARROWS                              ||
// =========================================================================
// Arrows for scroll menu after clicking into the menu button
void draw_Arrows(void) {
  leftArrow(0, 15 + 2);
  rightArrow(0, 45 + 5);

  upArrow(90, 15 + 2);
  downArrow(90, 45 + 4);
}

void leftArrow(int x, int y) {
  display.drawLine(x + 2, y - 2, x + 2, y + 2, 0xFFFF);
  display.drawLine(x + 1, y - 1, x + 1, y + 1, 0xFFFF);
  display.drawLine(x + 0, y - 0, x + 0, y + 0, 0xFFFF);
}

void rightArrow(int x, int y) {
  display.drawLine(x + 0, y - 2, x + 0, y + 2, 0xFFFF);
  display.drawLine(x + 1, y - 1, x + 1, y + 1, 0xFFFF);
  display.drawLine(x + 2, y - 0, x + 2, y + 0, 0xFFFF);
}

void upArrow(int x, int y) {
  display.drawLine(x + 0, y - 0, x + 4, y - 0, 0xFFFF);
  display.drawLine(x + 1, y - 1, x + 3, y - 1, 0xFFFF);
  display.drawLine(x + 2, y - 2, x + 2, y - 2, 0xFFFF);
}
void downArrow(int x, int y) {
  display.drawLine(x + 0, y + 0, x + 4, y + 0, 0xFFFF);
  display.drawLine(x + 1, y + 1, x + 3, y + 1, 0xFFFF);
  display.drawLine(x + 2, y + 2, x + 2, y + 2, 0xFFFF);
}
// =========================================================================
// ||                          ALARM DISPLAY                              ||
// =========================================================================
// =========================================================================
// ||                          ALARM DISPLAY                              ||
// =========================================================================
unsigned long lastBlinkMillis = 0;
const unsigned long blinkInterval = 2000;  // Blink every 1000 milliseconds (1 second)
bool isOn = false;
void display_Alarm_Notif(int hour, int minute) {
  SerialMonitorInterface.println("Entered function");

  // start the display
  display.begin();

  hours = rtc.getHours();
  minutes = rtc.getMinutes();
  
  SerialMonitorInterface.println(hours);
  SerialMonitorInterface.println(minutes);

  unsigned long currentMillis = millis();

  if (hour == hours && minute == minutes) {
    display.clearWindow(20, 45, 50, 16);
    SerialMonitorInterface.println("Correct Time");
    display.setFont(liberationSans_16ptFontInfo);
    display.setBrightness(10);
    display.setFlip(true);
    display.fontColor(TS_8b_White, TS_8b_Red);
    display.setCursor(5, 20);
    display.print("Wake up!!");
    //page_displayAlarm();
    /*if (isOn) {
      display.off();
    } else {
      display.on();
    }
      //isOn = !isOn;*/
  } else {
    display.clearScreen();
    switch_page();
  }
} 

void drawCircle(int x0, int y0, int radius, uint8_t color)
{
  int x = radius;
  int y = 0;
  int radiusError = 1-x;
 
  while(x >= y)
  {
    //drawPixel(x,y,color);//set pixel (x,y) to specified color. This is slow because we need to send commands setting the x and y, then send the pixel data.
    display.drawPixel(x + x0, y + y0, color);
    display.drawPixel(y + x0, x + y0, color);
    display.drawPixel(-x + x0, y + y0, color);
    display.drawPixel(-y + x0, x + y0, color);
    display.drawPixel(-x + x0, -y + y0, color);
    display.drawPixel(-y + x0, -x + y0, color);
    display.drawPixel(x + x0, -y + y0, color);
    display.drawPixel(y + x0, -x + y0, color);
    y++;
    if (radiusError<0)
    {
      radiusError += 2 * y + 1;
    }
    else
    {
      x--;
      radiusError += 2 * (y - x) + 1;
    }
  }
}

void drawHand(int xStart, int yStart, int angle, int radius, unsigned char color)

{
  static int sine[16] = {0, 27, 54, 79, 104, 128, 150, 171, 190, 201, 221, 233, 243, 250, 254, 255};
  int xEnd, yEnd, quadrant, x_flip, y_flip;

  // calculate which quadrant the hand lies in
  quadrant = angle/15 ;

  switch ( quadrant )
  {
    case 0 : x_flip = 1 ; y_flip = -1 ; break ;
    case 1 : angle = abs(angle-30) ; x_flip = y_flip = 1 ; break ;
    case 2 : angle = angle-30 ; x_flip = -1 ; y_flip = 1 ; break ;
    case 3 : angle = abs(angle-60) ; x_flip = y_flip = -1 ; break ; 
    default:  x_flip = y_flip = 1; // this should not happen
  }

  xEnd = xStart;
  yEnd = yStart;
  xEnd += (x_flip * (( sine[angle] * radius ) >> 8));
  yEnd += (y_flip * (( sine[15-angle] * radius ) >> 8));

  display.drawLine(xStart, yStart, xEnd, yEnd, color);
}

void displayClockTime()
{
  display.fontColor(TS_8b_White,TS_8b_Black); //Set the font color, font background
  display.setFont(thinPixel7_10ptFontInfo);
  //display.on();                           

 enum
 {
    clockCenterX = 55,
    clockCenterY = 22,
    clockCircleRadius = 20,
    clockCircleColor = TS_8b_Red,
    clockMinuteHandLength = 18,
    clockMinuteHandColor = TS_8b_White,
    clockHourHandLength = 8,
    clockHourHandColor = TS_8b_White,
    clockSecondHandLength = 18,
    clockSecondHandColor = TS_8b_Yellow
  };

  drawCircle(clockCenterX, clockCenterY, clockCircleRadius, clockCircleColor);
  
  for (int i = 1; i < 15; i++) // Display for 15*1000 milliseconds (15 seconds), update display each second
  {

    display.setFont(liberationSansNarrow_12ptFontInfo);  
    months = rtc.getMonth();
    days = rtc.getDay();
    years = rtc.getYear();
    hours = rtc.getHours();
    minutes = rtc.getMinutes();
    seconds = rtc.getSeconds();
    int hourAngle;
    if (hours >= 12)
      hourAngle = hours - 12;
    else
      hourAngle = hours;
    hourAngle *= 5;
    hourAngle += minutes / 12;
    
    
    drawHand(clockCenterX, clockCenterY, hourAngle, clockHourHandLength, clockHourHandColor);
    drawHand(clockCenterX, clockCenterY, minutes, clockMinuteHandLength, clockMinuteHandColor);
    drawHand(clockCenterX, clockCenterY, seconds, clockSecondHandLength, clockSecondHandColor);
    

    if (hours <= 12)
      brightness = hours + 3; // 0 hours = 3 brightness, noon = 15
    else if (hours >= 18)
      brightness = (24 - hours) * 2 + 2;  // 23 hours = 4 brightness, 18 hours = 14
    else
      brightness = 15; // full brightness all afternoon
    if (brightness < 3)
      brightness = 3;
    if (brightness > 15)
      brightness = 15;
    display.setBrightness(brightness);
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
    
    // Now erase the clock hands by drawing them in black.
    drawHand(clockCenterX, clockCenterY, hourAngle, clockHourHandLength, TS_8b_Black);
    drawHand(clockCenterX, clockCenterY, minutes, clockMinuteHandLength, TS_8b_Black);
    drawHand(clockCenterX, clockCenterY, seconds, clockSecondHandLength, TS_8b_Black);
  }


}

// =========================================================================
// ||                          SWITCH PAGES                              ||
// =========================================================================
void switch_page(void) {
  switch (currPage) {
    case LOCK_SCREEN: page_LockScreen(); break;
    case ROOT_MENU: page_RootMenu(); break;
    case SUB_MENU1: page_SubMenu1(); break;
    case SUB_MENU2: page_SubMenu2(); break;
    case SUB_MENU3: page_SubMenu3(); break;
    case WEATHER_SCREEN: page_Weather(); break;
    case TEST_SCREEN: page_Test(); break;
    //case GRAPH_SCREEN: page_Graph(); break;
    case HOOTHOOT_START_SCREEN: page_HootHootStart(); break;
    case HOOTHOOT_QUIZ_SCREEN: page_HootHootQuiz(); break;
    case HOOTHOOT_SUBMISSION_SCREEN: page_HootHootSubmission(); break;
    case ALARM_SCREEN: page_Alarm(); break;
    //case ATTENDANCE_SCREEN: page_Attendance(); break;
    case OPENAI_SCREEN: page_OpenAI(); break;
    case SILENTHELPER_SCREEN: page_SilentHelper(); break;
    case AUDIO_SCREEN: page_Audio(); break;
    case GAME_SCREEN: page_Game(); break;
    case OXIMETER_SCREEN: page_Oximeter(); break;
    //case TELEBOT_SCREEN: page_Telebot(); break;
  }
}

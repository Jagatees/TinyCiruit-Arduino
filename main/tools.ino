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

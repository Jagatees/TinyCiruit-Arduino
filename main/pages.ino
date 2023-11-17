// =========================================================================
// ||                          PAGE - WEATHER SCREEN                      ||
// =========================================================================
void page_Weather(void) {
  // flag for updating the display
  boolean updateDisplay = true;
  boolean updateDynamicSection = true;

  // tracks when entered top of loop
  uint32_t loopStartMs;

  //tracks button states
  boolean btn_Up_WasDown = false;
  boolean btn_Down_WasDown = false;
  boolean btn_Cancel_WasDown = false;

  // selected item pointer
  uint8_t sub_Pos = 1;

  String result;
  String temperature;
  String temperature_min;
  String temperature_max;
  String weather_condition;

  while(true){
    loopStartMs = millis();

    client.loop();

    if (dictionary.get("Weather/Response") != "") { /* Weather condition, Temperature now, Max temperature, Min temperature */
      result = dictionary.get("Weather/Response");

      int firstComma = result.indexOf(',');
      int secondComma = result.indexOf(',', firstComma + 1);
      int thirdComma = result.indexOf(',', secondComma + 1);

      weather_condition = result.substring(0, firstComma);
      temperature = result.substring(firstComma + 1, secondComma);
      temperature_max = result.substring(secondComma + 1, thirdComma);
      temperature_min = result.substring(thirdComma + 1);
    }

    // print the display
    if (updateDisplay) {

      // clear the update flag
      updateDisplay = false;
      // clear the display
      clearScreen();

      // start the display
      display.begin();
      display.setBrightness(10);  // Set brightness level (0-100)

      // print arrow buttons
      printBtnArrows();

      // menu title
      display.setCursor(0, 0);
      display.print("[ WEATHER SCREEN ]");
    }

    // update only a section of the screen
    if (updateDynamicSection) {
      updateDynamicSection = false;
      // call the weather function and get the returned string
      // print the items
      //display.setCursor(24, 32); // centered to the middle of screen

      if (dictionary.get("Weather/Response") == "") {
        display.setCursor(24, 32);  // centered to the middle of screen
        display.print("No data yet");
      } else {
        display.setCursor(0, 10);
        display.print("Temperature: " + temperature);

        display.setCursor(0, 20);
        display.print("Condition: " + weather_condition);

        display.setCursor(0, 30);
        display.print("Min: " + temperature_min);

        display.setCursor(0, 40);
        display.print("Max: " + temperature_max);
        //display.print(receivedPayload);
      }
    }

    // capture button down states
    if (btnIsDown(BTN_UP)) { btn_Up_WasDown = true; }
    if (btnIsDown(BTN_DOWN)) { btn_Down_WasDown = true; }
    if (btnIsDown(BTN_CANCEL)) { btn_Cancel_WasDown = true; }


    // move the pointer down
    if (btn_Down_WasDown && btnIsUp(BTN_DOWN)) {
      if (sub_Pos == SUB_MENU3_COUNT) {
        sub_Pos = 1;
      } else {
        sub_Pos++;
      }
      updateDynamicSection = true;
      btn_Down_WasDown = false;
    }

    // move the pointer up
    if (btn_Up_WasDown && btnIsUp(BTN_UP)) {
      if (sub_Pos == 1) {
        sub_Pos = SUB_MENU3_COUNT;
      } else {
        sub_Pos--;
      }
      updateDynamicSection = true;
      btn_Up_WasDown = false;
    }

    // move to the root menu
    if (btn_Cancel_WasDown && btnIsUp(BTN_CANCEL)) {
      currPage = SUB_MENU2;
      return;
    }
    // keep a specific pace
    while (millis() - loopStartMs < 25) { delay(2); }
  }
  // inner loop
  /*while (true) {


  }*/
}

// =========================================================================
// ||                     PAGE - HOOTHOOT START PAGE                      ||
// =========================================================================
void page_HootHootStart(void) {
  // flag for updating the display
  boolean updateDisplay = true;
  boolean updateDynamicSection = true;
  boolean readyToStartQuiz = false;

  // tracks when entered top of loop
  uint32_t loopStartMs;

  //tracks button states
  boolean btn_Up_WasDown = false;
  boolean btn_Down_WasDown = false;
  boolean btn_Accept_WasDown = false;
  boolean btn_Cancel_WasDown = false;

  // selected item pointer
  uint8_t sub_Pos = 1;

  // inner loop
  while (true) {
    loopStartMs = millis();

    client.loop();

    // print the display
    if (updateDisplay) {
      // clear the update flag
      updateDisplay = false;
      // clear the display
      clearScreen();

      // start the display
      display.begin();
      display.setBrightness(10);  // Set brightness level (0-100)

      // print arrow buttons
      printBtnArrows();

      // menu title
      display.setCursor(0, 0);
      display.print("[ HOOTHOOT START ]");
    }

    if (updateDynamicSection) {
      updateDynamicSection = false;
      // call the weather function and get the returned string
      // print the items
      display.setCursor(24, 32);

      client.loop();
      String result = dictionary.get("HootHoot/Start");

      if (result == "True") {
        readyToStartQuiz = true;
        display.print("Quiz is ready");
      } else if (result == "False") {
        display.print("Quiz is not ready");
      } else if (result == "") {
        display.print("No data yet");
      }
    }
    // capture button down states
    if (btnIsDown(BTN_UP)) { btn_Up_WasDown = true; }
    if (btnIsDown(BTN_DOWN)) { btn_Down_WasDown = true; }
    if (btnIsDown(BTN_ACCEPT)) { btn_Accept_WasDown = true; }
    if (btnIsDown(BTN_CANCEL)) { btn_Cancel_WasDown = true; }

    // move the pointer down
    if (btn_Down_WasDown && btnIsUp(BTN_DOWN)) {
      updateDynamicSection = true;
      btn_Down_WasDown = false;
    }

    // move the pointer up
    if (btn_Up_WasDown && btnIsUp(BTN_UP)) {
      updateDynamicSection = true;
      btn_Up_WasDown = false;
    }
    // move to the root menu
    if (btn_Accept_WasDown && btnIsUp(BTN_ACCEPT)) {
      if (readyToStartQuiz) { currPage = HOOTHOOT_QUIZ_SCREEN; }
      return;
    }

    // move to the root menu
    if (btn_Cancel_WasDown && btnIsUp(BTN_CANCEL)) {
      currPage = SUB_MENU1;
      return;
    }

    // keep a specific pace
    while (millis() - loopStartMs < 25) { delay(2); }
  }
}

// =========================================================================
// ||                     PAGE - HOOTHOOT QUIZ PAGE                      ||
// =========================================================================
String user_input;

void page_HootHootQuiz(void) {
  // flag for updating the display
  boolean updateDisplay = true;
  boolean updateDynamicSection = true;
  boolean responseSent = false;
  // tracks when entered top of loop
  uint32_t loopStartMs;

  //tracks button states
  boolean btn_Down_WasDown = false;
  boolean btn_Up_WasDown = false;

  boolean btn_Down_Disabled = false;
  boolean btn_Up_Disabled = false;

  // selected item pointer
  uint8_t sub_Pos = 1;
  String result1;
  String result2;

  // inner loop
  while (true) {
    loopStartMs = millis();

    client.loop();

    if (dictionary.get("HootHoot/Question1/Option1") == "A") {
      result1 = dictionary.get("HootHoot/Question1/Option1");
    }

    if (dictionary.get("HootHoot/Question1/Option2") == "B") {
      result2 = dictionary.get("HootHoot/Question1/Option2");
    }


    // print the display
    if (updateDisplay) {
      // clear the update flag
      updateDisplay = false;
      // clear the display
      clearScreen();

      // start the display
      display.begin();
      display.setBrightness(10);  // Set brightness level (0-100)

      // print arrow buttons
      printBtnArrows();

      // menu title
      display.setCursor(0, 0);
      display.print("[ HOOTHOOT QUIZ ]");
    }

    if (updateDynamicSection) {
      updateDynamicSection = false;
      // call the weather function and get the returned string
      // print the items
      display.setCursor(0, 32);
      if (user_input != "") {

      
      }

      display.setCursor(24, 45);
      display.print(result1);

      display.setCursor(64, 45);
      display.print(result2);
    }
    // capture button down states
    if (btnIsDown(BTN_DOWN)) { btn_Down_WasDown = true; }
    if (btnIsDown(BTN_UP)) { btn_Up_WasDown = true; }


    // move the pointer up
    if (btn_Down_WasDown && btnIsUp(BTN_DOWN)) {
      user_input = result2;
      updateDynamicSection = true;
      btn_Down_WasDown = false;
      if (user_input == "B") {
          client.publish("HootHoot/Answer", "B");
                responseSent = true;

        }
      if (responseSent) {
        currPage = HOOTHOOT_SUBMISSION_SCREEN;
        return;
      }
  
    }
    // move to the root menu
    if (btn_Up_WasDown && btnIsUp(BTN_UP)) {
      user_input = result1;
      updateDynamicSection = true;
      btn_Up_WasDown = false;

        if (user_input == "A") {
          client.publish("HootHoot/Answer", "A");
                responseSent = true;
        }

      if (responseSent) {
      currPage = HOOTHOOT_SUBMISSION_SCREEN;
      return;
      }

    }

    // keep a specific pace
    while (millis() - loopStartMs < 25) { delay(2); }
  }
}
// =========================================================================
// ||                PAGE - HOOTHOOT SUBMISSION PAGE                      ||
// =========================================================================
void page_HootHootSubmission(void) {
  // flag for updating the display
  boolean updateDisplay = true;
  boolean updateDynamicSection = true;

  // tracks when entered top of loop
  uint32_t loopStartMs;

  //tracks button states
  boolean btn_Down_WasDown = false;
  boolean btn_Up_WasDown = false;
  boolean btn_Cancel_WasDown = false;

  // inner loop
  while (true) {
    loopStartMs = millis();

    client.loop();
    String result = dictionary.get("HootHoot/Question1/ProfAnswer");

    // print the display
    if (updateDisplay) {
      // clear the update flag
      updateDisplay = false;
      // clear the display
      clearScreen();

      // start the display
      display.begin();
      display.setBrightness(10);  // Set brightness level (0-100)

      // print arrow buttons
      printBtnArrows();

      // menu title
      display.setCursor(0, 0);
      display.print("[ HOOTHOOT SUBMISSION ]");
    }

    if (updateDynamicSection) {
      updateDynamicSection = false;
      // call the weather function and get the returned string
      // print the items
      display.setCursor(0, 32);
      if (user_input != "") {
        display.print("You have chosen: " + user_input);
      }

      if (result != "") {
        display.setCursor(0, 45);
        display.print("Correct answer: " + result);
      } else {
        display.setCursor(0, 45);
        display.print("Waiting for answer...");
      }
    }
    // capture button down states
    if (btnIsDown(BTN_DOWN)) { btn_Down_WasDown = true; }
    if (btnIsDown(BTN_UP)) { btn_Up_WasDown = true; }
    if (btnIsDown(BTN_CANCEL)) { btn_Cancel_WasDown = true; }


    // move the pointer up
    if (btn_Down_WasDown && btnIsUp(BTN_DOWN)) {
      updateDynamicSection = true;
      btn_Down_WasDown = false;
    }
    // move to the root menu
    if (btn_Up_WasDown && btnIsUp(BTN_UP)) {
      updateDynamicSection = true;
      btn_Up_WasDown = false;
    }

    // move to the root menu
    if (btn_Cancel_WasDown && btnIsUp(BTN_CANCEL)) {
      currPage = SUB_MENU1;
      return;
    }

    // keep a specific pace
    while (millis() - loopStartMs < 25) { delay(2); }
  }
}

// =========================================================================
// ||                          PAGE - ATTENDANCE PAGE                     || 
// =========================================================================  
void page_Attendance (void) {
  // flag for updating the display
  boolean updateDisplay = true;
  boolean updateDynamicSection = true;

  // tracks when entered top of loop
  uint32_t loopStartMs;

  String name;
  //tracks button states
  boolean btn_Up_WasDown = false;
  boolean btn_Cancel_WasDown = false;

  // selected item pointer
  uint8_t sub_Pos = 1;
  // inner loop
  while (true) {
    loopStartMs = millis();

    // print the display
    if (updateDisplay) {
      // clear the update flag
      updateDisplay = false;
      // clear the display
      clearScreen();

      // start the display
      display.begin();
      display.setBrightness(10); // Set brightness level (0-100)

      // print arrow buttons
      printBtnArrows();

      // menu title
      display.setCursor(0, 0); 
      display.print("[ ATTENDANCE ]");
    }

    if (updateDynamicSection) {
      updateDynamicSection = false;
      // call the weather function and get the returned string
      // print the items
      display.setCursor(24, 32);
      display.print(name); 

    }
    // capture button down states
    if (btnIsDown(BTN_UP)) {btn_Up_WasDown = true;}
    if (btnIsDown(BTN_CANCEL)) {btn_Cancel_WasDown = true;}

    // move the pointer up
    if (btn_Up_WasDown && btnIsUp(BTN_UP)) {
      client.publish("Attendance","Vanessa");
      name = "Vanessa";
      display.setCursor(0, 42);
      display.print("Attendance taken!");

      updateDynamicSection = true;
      btn_Up_WasDown = false;
      
    } 

    // move to the root menu
    if (btn_Cancel_WasDown && btnIsUp(BTN_CANCEL)) { currPage = SUB_MENU1; return; }

    // keep a specific pace
    while (millis() - loopStartMs < 25) { delay(2); }
  }
}
// =========================================================================
// ||                  PAGE - SILENTHELPER PAGE                           ||
// =========================================================================
void page_SilentHelper(void) {
  // flag for updating the display
  boolean updateDisplay = true;
  boolean updateDynamicSection = true;
  boolean sentResponse = false;

  // tracks when entered top of loop
  uint32_t loopStartMs;

  //tracks button states
  boolean btn_Down_WasDown = false;
  boolean btn_Cancel_WasDown = false;

  String prof_result;
  String openai_result;

  // inner loop
  while (true) {
    loopStartMs = millis();

    client.loop();

    // print the display
    if (updateDisplay) {
      // clear the update flag
      updateDisplay = false;
      // clear the display
      clearScreen();

      // start the display
      display.begin();
      display.setBrightness(10);  // Set brightness level (0-100)

      // print arrow buttons
      printBtnArrows();

      // menu title
      display.setCursor(0, 0);
      display.print("[ SILENT HELPER ]");
    }

    if (updateDynamicSection) {
      updateDynamicSection = false;

      display.setCursor(0, 22);
      display.print("Do you require help?");

      display.setCursor(0, 32); 
      display.print("Press down button");

      if (sentResponse) {
        client.publish("Studenthelp" , "Jake");
        display.setCursor(0, 42);
        display.print("Response sent!");
      }

      // 2 buttons, one to respond with openAI_result, one go back to main menu
    }

    // capture button down states
    if (btnIsDown(BTN_DOWN)) { btn_Down_WasDown = true; }
    if (btnIsDown(BTN_CANCEL)) { btn_Cancel_WasDown = true; }

    // move the pointer up
    if (btn_Down_WasDown && btnIsUp(BTN_DOWN)) {
      sentResponse = true;
      updateDynamicSection = true;
      btn_Down_WasDown = false;
    }

    // move to the root menu
    if (btn_Cancel_WasDown && btnIsUp(BTN_CANCEL)) {
      currPage = SUB_MENU1;
      return;
    }

    // keep a specific pace
    while (millis() - loopStartMs < 25) { delay(2); }
  }
}
// =========================================================================
// ||                  PAGE - AUDIO PAGE                           ||
// =========================================================================
void page_Audio(void) {
  // flag for updating the display
  boolean updateDisplay = true;
  boolean updateDynamicSection = true;

  // tracks when entered top of loop
  uint32_t loopStartMs;

  //tracks button states
  boolean btn_Down_WasDown = false;
  boolean btn_Cancel_WasDown = false;

  // selected item pointer
  uint8_t sub_Pos = 1;

  // inner loop
  while (true) {
    loopStartMs = millis();

    client.loop();

    
    // print the display
    if (updateDisplay) {
      // clear the update flag
      updateDisplay = false;
      // clear the display
      clearScreen();

      // start the display
      display.begin();
      display.setBrightness(10);  // Set brightness level (0-100)

      // print arrow buttons
      printBtnArrows();

      // menu title
      display.setCursor(0, 0);
      display.print("[ AUDIO ]");
    }

    if (updateDynamicSection) {
      updateDynamicSection = false;

      display.setCursor(0, 20);
      display.print("Select a song to play:");

      display.setCursor(0, 30);
      printSelected(1, sub_Pos); display.print("Song 1");

      display.setCursor(0, 42);
      display.print("Current Song: Song 1");
    }

    // capture button down states
    if (btnIsDown(BTN_DOWN)) { btn_Down_WasDown = true; }
    if (btnIsDown(BTN_CANCEL)) { btn_Cancel_WasDown = true; }

    // move the pointer up
    if (btn_Down_WasDown && btnIsUp(BTN_DOWN)) {
      getAudio();
      updateDynamicSection = true;
      btn_Down_WasDown = false;
    }

    // move to the root menu
    if (btn_Cancel_WasDown && btnIsUp(BTN_CANCEL)) {
      currPage = SUB_MENU2;
      return;
    }

    // keep a specific pace
    while (millis() - loopStartMs < 25) { delay(2); }
  }
}
// =========================================================================
// ||                         PAGE - ALARM PAGE                           ||
// =========================================================================
int oldMinutes = 0;
unsigned long previousMillis = 0;
const long interval = 1000;  // Set the interval in milliseconds (adjust as needed)

void set_alarm(int hour, int minute) {
  alarmHour = hour;
  alarmMinute = minute;
  alarmSet = true;
  
  SerialMonitorInterface.print(alarmHour);
  SerialMonitorInterface.print(alarmMinute);
}

bool check_alarm(void) {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    minutes = rtc.getMinutes();
    SerialMonitorInterface.println("Minutes have changed");
    SerialMonitorInterface.println(minutes);

    if (minutes != oldMinutes) {
      oldMinutes = minutes;
      // Check and compare rtc time to set alarm
      if (true) { // Adjust the condition based on your requirements
        SerialMonitorInterface.println("Entered alarmSet");
        // When comparison is correct, turn on the buzzer or run alarm code
        return true;
      }
    } else {
      return false;
    }
  }
}

void page_Alarm(void) {
  // flag for updating the display
  boolean updateDisplay = true;
  boolean updateDynamicSection = true;

  // tracks when entered top of loop
  uint32_t loopStartMs;

  //tracks button states
  boolean btn_Up_WasDown = false;
  boolean btn_Down_WasDown = false;
  boolean btn_Cancel_WasDown = false;
  boolean btn_Accept_WasDown = false;

  int setHour = 0;
  int setMinute = 0;  
  bool alarmSetResponse = false;
  
  bool isOn = true;

  // inner loop
  while (true) {
    
    loopStartMs = millis();
    hours = rtc.getHours();
    minutes = rtc.getMinutes();

    // print the display
    if (updateDisplay) {
      // clear the update flag
      updateDisplay = false;
      // clear the display
      clearScreen();

      // start the display
      display.begin();
      display.setBrightness(10);  // Set brightness level (0-100)

      // print arrow buttons
      printBtnArrows();

      // menu title
      display.setCursor(0, 0);
      display.print("[ ALARM ]");

      display.setCursor(24, 10);
      display.print("Set Alarm");
    }

    if (updateDynamicSection) {
      updateDynamicSection = false;
      display.setCursor(24, 20);
      display.print("HH : MM");
      // Display current input
      display.setCursor(24, 30);
      display.print(setHour);

      display.print(" : ");

      display.print(setMinute);

      if (alarmSetResponse) {
        display.setCursor(24, 50);
        display.print("Alarm Set");
      }

      if (hours == alarmHour && minutes == alarmMinute && alarmSet == true) { //
          //if (!isOn) { isOn = true; }
          //display.clearWindow(20, 45, 50, 16);
          SerialMonitorInterface.println("Correct Time");
          SerialMonitorInterface.println(isOn);
          //display.setCursor(24, 50);
          //display.print("Wake up!!");

          if (isOn) {
            display.setBrightness(0);
            delay(500);
            updateDynamicSection = true;
          } else {
            display.setBrightness(20);
            display.setCursor(24, 50);
            display.print("Wake up!!");
            delay(500);
            updateDynamicSection = true;
          }

          isOn = !isOn;
      }
      

    }
    // capture button down states
    if (btnIsDown(BTN_UP)) { btn_Up_WasDown = true; }
    if (btnIsDown(BTN_DOWN)) { btn_Down_WasDown = true; }
    if (btnIsDown(BTN_CANCEL)) { btn_Cancel_WasDown = true; }
    if (btnIsDown(BTN_ACCEPT)) { btn_Accept_WasDown = true; }

    // move the pointer up
    if (btn_Up_WasDown && btnIsUp(BTN_UP)) {
      setHour = (setHour + 1) % 24;
      updateDynamicSection = true;
      btn_Up_WasDown = false;
    }

    // move the pointer up
    if (btn_Down_WasDown && btnIsUp(BTN_DOWN)) {
      setMinute = (setMinute + 10) % 60;
      updateDynamicSection = true;
      btn_Down_WasDown = false;
    }

    // move to the root menu
    if (btn_Cancel_WasDown && btnIsUp(BTN_CANCEL)) {
      currPage = SUB_MENU2;
      return;
    }

    // move to the root menu
    if (btn_Accept_WasDown && btnIsUp(BTN_ACCEPT)) {
      set_alarm(setHour, setMinute);
      alarmSetResponse = true;
      updateDynamicSection = true;
      btn_Accept_WasDown = false;
    }
  // keep a specific pace
  while (millis() - loopStartMs < 25) { delay(2); }

  }

}
// =========================================================================
// ||                      PAGE - OXIMETER PAGE                           ||
// =========================================================================
void page_Oximeter(void) {

  // flag for updating the display
  boolean updateDisplay = true;
  boolean updateDynamicSection = true;

  // tracks when entered top of loop
  uint32_t loopStartMs;

  //tracks button states
  boolean btn_Down_WasDown = false;
  boolean btn_Cancel_WasDown = false;

  // selected item pointer
  uint8_t sub_Pos = 1;



  // inner loop
  while (true) {
    loopStartMs = millis();
    client.loop();

  if (pulseSensor.update()) {
      SerialMonitorInterface.print("Update");
    if (pulseSensor.pulseValid()) {
      SerialMonitorInterface.print("PulseVaild");
      SerialMonitorInterface.print(pulseSensor.temperature());
      SerialMonitorInterface.print("\t");
      SerialMonitorInterface.print(pulseSensor.BPM());
      SerialMonitorInterface.print("\t");
      SerialMonitorInterface.print(pulseSensor.oxygen());
      SerialMonitorInterface.print("\t");
      SerialMonitorInterface.println(pulseSensor.cardiogram() * 10.0);
      updateDynamicSection = true;
    } else {
      SerialMonitorInterface.print("Else");
      SerialMonitorInterface.print(pulseSensor.temperature());
      SerialMonitorInterface.print("\t");
      SerialMonitorInterface.println("-\t-\t-");
      updateDynamicSection = true;
    }
  }

    // print the display
    if (updateDisplay) {
      // clear the update flag
      updateDisplay = false;
      // clear the display
      clearScreen();

      // start the display
      display.begin();
      display.setBrightness(10);  // Set brightness level (0-100)

      // print arrow buttons
      printBtnArrows();

      // menu title
      display.setCursor(0, 0);
      display.print("[ OXIMETER ]");
    }

    if (updateDynamicSection) {
      updateDynamicSection = false;
      // Display SpO2

      display.setCursor(10, 10);
      display.print("Oxygen:");
      display.print(pulseSensor.oxygen());

      // Display heart rate
      display.setCursor(10, 20);
      display.print("Temp: ");
      display.print(pulseSensor.temperature());
      display.setCursor(10, 30);
      display.print("BPM");
      display.print(pulseSensor.BPM());
      display.setCursor(10, 40);
      display.print("Cardiogram");
      display.print(pulseSensor.cardiogram() * 10.0);

    }

    // capture button down states
    if (btnIsDown(BTN_DOWN)) { btn_Down_WasDown = true; }
    if (btnIsDown(BTN_CANCEL)) { btn_Cancel_WasDown = true; }

    // move the pointer up
    if (btn_Down_WasDown && btnIsUp(BTN_DOWN)) {
      updateDynamicSection = true;
      btn_Down_WasDown = false;
    }

    // move to the root menu
    if (btn_Cancel_WasDown && btnIsUp(BTN_CANCEL)) {
      currPage = SUB_MENU2;
      return;
    }

    // keep a specific pace
    while (millis() - loopStartMs < 25) { delay(2); }
  }
}
// =========================================================================
// ||                         PAGE - TELEBOT PAGE                           ||
// =========================================================================
void page_Telebot(void) {
  // flag for updating the display
  boolean updateDisplay = true;
  boolean updateDynamicSection = true;
  boolean sentResponse = false;
  boolean readyToStart = false;
  // tracks when entered top of loop
  uint32_t loopStartMs;

  //tracks button states
  boolean btn_Up_WasDown = false;
  boolean btn_Down_WasDown = false;
  boolean btn_Cancel_WasDown = false;
  boolean btn_Accept_WasDown = false;

  String tele_announcement;
  String tele_jake;
  String result;

  String firstResponse;
  String secondResponse;
  String thirdResponse;
  String fourthResponse;

  String user_input;
  // inner loop
  while (true) {
    loopStartMs = millis();

    client.loop();

// HootHoot/Start True
    if (dictionary.get("tele/Announcement") != "") {
      tele_announcement = dictionary.get("tele/Announcement");

   if (dictionary.get("tele/SuggestedResponse") != "") { /* Weather condition, Temperature now, Max temperature, Min temperature */
      result = dictionary.get("tele/SuggestedResponse");

      int firstComma = result.indexOf(',');
      int secondComma = result.indexOf(',', firstComma + 1);
      int thirdComma = result.indexOf(',', secondComma + 1);

      firstResponse = result.substring(0, firstComma);
      secondResponse = result.substring(firstComma + 1, secondComma);
      thirdResponse = result.substring(secondComma + 1, thirdComma);
      fourthResponse = result.substring(thirdComma + 1);

      readyToStart = true;
    } else {
display.print("No data Yet");
    }
    } else {
      display.print("No data Yet");
    }
    
    tele_jake = dictionary.get("tele/Jake");
    
    // print the display
    if (updateDisplay) {
      // clear the update flag
      updateDisplay = false;
      // clear the display
      clearScreen();

      // start the display
      display.begin();
      display.setBrightness(10);  // Set brightness level (0-100)

      // print arrow buttons
      printBtnArrows();

      // menu title
      display.setCursor(0, 0);
      display.print("[ TELEBOT ]");
    }

    if (updateDynamicSection) {
      updateDynamicSection = false;
      display.setCursor(10,22);
      display.print(tele_announcement);

      if (dictionary.get("tele/SuggestedResponse") == "") {
        firstResponse = "Yes";
        display.setCursor(24, 45);
        display.print("Yes");

        secondResponse = "No";
        display.setCursor(64, 45);
        display.print("No");
      } else {
        display.setCursor(24, 45);
        display.print(firstResponse);

        display.setCursor(64, 45);
        display.print(secondResponse);
      }



      /*if (tele_annoucement != "") {
        display.setCursor(0, 22);
        display.print(tele_annoucement);
      }

      display.setCursor(0, 32); 
      display.print(openai_result);
      if (openai_result != "") {
        if (sentResponse) {
          display.setCursor(0, 45);
          display.print("Response sent!");
        }

      }*/
      // 2 buttons, one to respond with openAI_result, one go back to main menu
    }

    // capture button down states
    if (btnIsDown(BTN_UP)) { btn_Up_WasDown = true; }
    if (btnIsDown(BTN_DOWN)) { btn_Down_WasDown = true; }
    if (btnIsDown(BTN_CANCEL)) { btn_Cancel_WasDown = true; }
    if (btnIsDown(BTN_ACCEPT)) { btn_Accept_WasDown = true; }

    // move the pointer up
    if (btn_Down_WasDown && btnIsUp(BTN_DOWN)) {
      user_input = secondResponse + "By Jake";
      sentResponse = true;
      client.publish("tele/Request", user_input.c_str());
      updateDynamicSection = true;
      btn_Down_WasDown = false;
    }

        // move the pointer up
    if (btn_Up_WasDown && btnIsUp(BTN_UP)) {
      user_input = firstResponse + "By Jake";
      sentResponse = true;
      client.publish("tele/Request", user_input.c_str());

      updateDynamicSection = true;
      btn_Up_WasDown = false;
    }

    // move to the root menu
    if (btn_Cancel_WasDown && btnIsUp(BTN_CANCEL)) {
      currPage = SUB_MENU1;
      return;
    }

    // keep a specific pace
    while (millis() - loopStartMs < 25) { delay(2); }
  }
}
// =========================================================================
// ||                         PAGE - SMS PAGE                           ||
// =========================================================================
void page_SMS(void) {
  // flag for updating the display
  boolean updateDisplay = true;
  boolean updateDynamicSection = true;
  boolean sentResponse = false;
  boolean readyToStart = false;
  // tracks when entered top of loop
  uint32_t loopStartMs;

  //tracks button states
  boolean btn_Up_WasDown = false;
  boolean btn_Down_WasDown = false;
  boolean btn_Cancel_WasDown = false;
  boolean btn_Accept_WasDown = false;

  String tele_announcement;
  String tele_jake;
  String result;

  String firstResponse;
  String secondResponse;
  String thirdResponse;
  String fourthResponse;

  String user_input;
  // inner loop
  while (true) {
    loopStartMs = millis();

    client.loop();

    // HootHoot/Start True
    if (dictionary.get("tele/Announcement") != "") {
      tele_announcement = dictionary.get("tele/Announcement");

      if (dictionary.get("tele/SuggestedResponse") != "") { /* Weather condition, Temperature now, Max temperature, Min temperature */
        result = dictionary.get("tele/SuggestedResponse");

        int firstComma = result.indexOf(',');
        int secondComma = result.indexOf(',', firstComma + 1);
        int thirdComma = result.indexOf(',', secondComma + 1);

        firstResponse = result.substring(0, firstComma);
        secondResponse = result.substring(firstComma + 1, secondComma);
        thirdResponse = result.substring(secondComma + 1, thirdComma);
        fourthResponse = result.substring(thirdComma + 1);

        readyToStart = true;
      } else {
        display.print("No data Yet");
      }
    } else {
      display.print("No data Yet");
    }
    
    tele_jake = dictionary.get("tele/Jake");
    
    // print the display
    if (updateDisplay) {
      // clear the update flag
      updateDisplay = false;
      // clear the display
      clearScreen();

      // start the display
      display.begin();
      display.setBrightness(10);  // Set brightness level (0-100)

      // print arrow buttons
      printBtnArrows();

      // menu title
      display.setCursor(0, 0);
      display.print("[ TELEBOT ]");
    }

    if (updateDynamicSection) {
      updateDynamicSection = false;
      display.setCursor(10,22);
      display.print(tele_announcement);

      if (dictionary.get("tele/SuggestedResponse") == "") {
        firstResponse = "Yes";
        display.setCursor(24, 45);
        display.print("Yes");

        secondResponse = "No";
        display.setCursor(64, 45);
        display.print("No");
      } else {
        display.setCursor(24, 45);
        display.print(firstResponse);

        display.setCursor(64, 45);
        display.print(secondResponse);
      }
    }

    // capture button down states
    if (btnIsDown(BTN_UP)) { btn_Up_WasDown = true; }
    if (btnIsDown(BTN_DOWN)) { btn_Down_WasDown = true; }
    if (btnIsDown(BTN_CANCEL)) { btn_Cancel_WasDown = true; }
    if (btnIsDown(BTN_ACCEPT)) { btn_Accept_WasDown = true; }

    // move the pointer up
    if (btn_Down_WasDown && btnIsUp(BTN_DOWN)) {
      user_input = secondResponse + "By Jake";
      sentResponse = true;
      client.publish("tele/Request", user_input.c_str());
      updateDynamicSection = true;
      btn_Down_WasDown = false;
    }

        // move the pointer up
    if (btn_Up_WasDown && btnIsUp(BTN_UP)) {
      user_input = firstResponse + "By Jake";
      sentResponse = true;
      client.publish("tele/Request", user_input.c_str());

      updateDynamicSection = true;
      btn_Up_WasDown = false;
    }

    // move to the root menu
    if (btn_Cancel_WasDown && btnIsUp(BTN_CANCEL)) {
      currPage = SUB_MENU1;
      return;
    }

    // keep a specific pace
    while (millis() - loopStartMs < 25) { delay(2); }
  }
}
// =========================================================================
// ||               PAGE - GOOGLE CALENDAR PAGE                           ||
// =========================================================================
void page_GoogleCalendar(void) {
  // flag for updating the display
  boolean updateDisplay = true;
  boolean updateDynamicSection = true;

  // tracks when entered top of loop
  uint32_t loopStartMs;

  //tracks button states
  boolean btn_Cancel_WasDown = false;

  String today_event;
  String today_date;

  // inner loop
  while (true) {
    loopStartMs = millis();

    client.loop();

    /*if (dictionary.get("Weather/Response") != "") {
      result = dictionary.get("Weather/Response");

      int firstComma = result.indexOf(',');
      int secondComma = result.indexOf(',', firstComma + 1);
      int thirdComma = result.indexOf(',', secondComma + 1);

      weather_condition = result.substring(0, firstComma);
      temperature = result.substring(firstComma + 1, secondComma);
      temperature_max = result.substring(secondComma + 1, thirdComma);
      temperature_min = result.substring(thirdComma + 1);
    }*/
    //today_event = dictionary.get("Announcement/Prof/Topic");

    // print the display
    if (updateDisplay) {
      // clear the update flag
      updateDisplay = false;
      // clear the display
      clearScreen();

      // start the display
      display.begin();
      display.setBrightness(10);  // Set brightness level (0-100)

      // print arrow buttons
      printBtnArrows();

      // menu title
      display.setCursor(0, 0);
      display.print("[ GOOGLE CALENDAR ]");
    }

    if (updateDynamicSection) {
      updateDynamicSection = false;

      if (today_event != "") {
        display.print("");
      } else {
        display.print("No Events today");
      }


      /*
      if (prof_result != "") {
        display.setCursor(0, 22);
        display.print(prof_result);
      }
      display.setCursor(0, 32); 
      display.print(openai_result);
      if (openai_result != "") {
        if (sentResponse) {
          display.setCursor(0, 45);
          display.print("Response sent!");
        }

      }*/
      // 2 buttons, one to respond with openAI_result, one go back to main menu
    }

    // capture button down states
    if (btnIsDown(BTN_CANCEL)) { btn_Cancel_WasDown = true; }


    // move to the root menu
    if (btn_Cancel_WasDown && btnIsUp(BTN_CANCEL)) {
      currPage = SUB_MENU1;
      return;
    }

    // keep a specific pace
    while (millis() - loopStartMs < 25) { delay(2); }
  }
}

// =========================================================================
// ||                          PAGE - GAME PAGE                           ||
// =========================================================================
#define	BLACK           0x00
#define	BLUE            0xE0
#define	RED             0x03
#define	GREEN           0x1C
#define	DGREEN          0x0C
#define YELLOW          0x1F
#define WHITE           0xFF
#define ALPHA           0xFE
#define	BROWN           0x32

unsigned char flappyBird[]={
  ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,
  ALPHA,ALPHA,ALPHA,ALPHA,BLACK,BLACK,WHITE,WHITE,WHITE,BLACK,WHITE,WHITE,BLACK,ALPHA,ALPHA,ALPHA,ALPHA,
  ALPHA,ALPHA,ALPHA,BLACK,WHITE,WHITE,YELLOW,YELLOW,BLACK,WHITE,WHITE,WHITE,WHITE,BLACK,ALPHA,ALPHA,ALPHA,
  ALPHA,ALPHA,BLACK,WHITE,YELLOW,YELLOW,YELLOW,YELLOW,BLACK,WHITE,WHITE,WHITE,BLACK,WHITE,BLACK,ALPHA,ALPHA,
  ALPHA,BLACK,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,BLACK,WHITE,WHITE,WHITE,BLACK,WHITE,BLACK,ALPHA,ALPHA,
  ALPHA,BLACK,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,BLACK,WHITE,WHITE,WHITE,WHITE,BLACK,ALPHA,ALPHA,
  ALPHA,BLACK,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,ALPHA,
  ALPHA,BLACK,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,BLACK,RED,RED,RED,RED,RED,RED,BLACK,
  ALPHA,ALPHA,BLACK,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,BLACK,RED,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,
  ALPHA,ALPHA,BLACK,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,BLACK,RED,RED,RED,RED,RED,BLACK,ALPHA,
  ALPHA,ALPHA,ALPHA,BLACK,BLACK,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,BLACK,BLACK,BLACK,BLACK,BLACK,ALPHA,ALPHA,
  ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,BLACK,BLACK,BLACK,BLACK,BLACK,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA
};
//7x8
unsigned char wingUp[]={
  ALPHA,BLACK,BLACK,BLACK,BLACK,ALPHA,ALPHA,
  BLACK,WHITE,WHITE,WHITE,WHITE,BLACK,ALPHA,
  BLACK,WHITE,WHITE,WHITE,WHITE,WHITE,BLACK,
  BLACK,YELLOW,WHITE,WHITE,WHITE,YELLOW,BLACK,
  ALPHA,BLACK,YELLOW,YELLOW,YELLOW,BLACK,ALPHA,
  ALPHA,ALPHA,BLACK,BLACK,BLACK,ALPHA,ALPHA,
  ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,
  ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,
};
unsigned char wingMid[]={
  ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,
  ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,
  ALPHA,BLACK,BLACK,BLACK,BLACK,BLACK,ALPHA,
  BLACK,WHITE,WHITE,WHITE,WHITE,WHITE,BLACK,
  BLACK,YELLOW,WHITE,WHITE,WHITE,YELLOW,BLACK,
  ALPHA,BLACK,BLACK,BLACK,BLACK,BLACK,ALPHA,
  ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,
  ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,
};
unsigned char wingDown[]={
  ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,
  ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,
  ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,
  ALPHA,BLACK,BLACK,BLACK,BLACK,BLACK,ALPHA,
  BLACK,YELLOW,WHITE,WHITE,WHITE,YELLOW,BLACK,
  BLACK,WHITE,WHITE,WHITE,WHITE,BLACK,ALPHA,
  BLACK,WHITE,WHITE,YELLOW,BLACK,ALPHA,ALPHA,
  ALPHA,BLACK,BLACK,BLACK,BLACK,ALPHA,ALPHA,
};
char wingHeight=8;
char wingWidth=7;

char flappyBirdX=25;
char flappyBirdY=30;
char flappyBirdHeight=12;
char flappyBirdWidth=17;
char amtPipes=4;
char PipeHeight[]={10,15,20,25,25,10,10};
char pipeOffsetX=0;
const char pipeWidth=10;
const char pipeSpacingX=26;
const char pipeSpacingY=26;
unsigned char lineBuffer[16+96+(pipeSpacingX+pipeWidth)];
unsigned char BGcolor=BLUE;
unsigned char pipeColor=GREEN;
char wingPos=1;
char cloudY[]={5,15,30,25,5,0};
char cloudOffsetX=0;
const char cloudWidth=15;
const char cloudSpacingX=30;
unsigned char cloudColor=WHITE;
char groundY=52;
char groundOffsetX=0;
char birdYmod=1;
unsigned long framecount=0;
int score = 0;
char brightnessChanged=0;
char brightness=5;
char movePipe=4;
char movePipeMod=1;
bool passedThrough = false;


void setBuffer(char i, char amt, unsigned char color){
  char endbyte=i+amt;
  while(i<endbyte)
    lineBuffer[i++]=color;
}

void updateDisplay(){
  display.goTo(0,0);
  display.startData();
  

  for(unsigned char y=0;y<groundY;y++){
    
    for(unsigned char i=16;i<112;i++)
      lineBuffer[i]=BGcolor;
      
    int x=cloudOffsetX;
    char cloud=0;
    while(x<16+96){ 				//displays clouds
      if(y>cloudY[cloud] && y<cloudY[cloud]+8){
        if(y<cloudY[cloud]+2 || y>cloudY[cloud]+6)
          setBuffer(x+1, cloudWidth-2, cloudColor);
        else
          setBuffer(x, cloudWidth, cloudColor);
      }
      cloud++;
      x+=(cloudSpacingX+cloudWidth);
    }
    
    x=pipeOffsetX;
    char pipe=0;
    while(x<16+96){					//displays pipes
      if(y<PipeHeight[pipe] || y>PipeHeight[pipe]+pipeSpacingY){
        if(y<PipeHeight[pipe]-4 || y>PipeHeight[pipe]+pipeSpacingY+4)
          setBuffer(x, pipeWidth, pipeColor);
        else
          setBuffer(x-1, pipeWidth+2, pipeColor);
      }
      pipe++;
      x+=(pipeSpacingX+pipeWidth);
    }
    
    if(y>=flappyBirdY && y<flappyBirdY+flappyBirdHeight){ 	//displays bird body
      int offset=(y-flappyBirdY)*flappyBirdWidth;
      for(int i=0;i<flappyBirdWidth;i++){
        unsigned char color=flappyBird[offset+i];
        if(color!=ALPHA)
          lineBuffer[16+flappyBirdX+i]=color;
      }
    }
    char wingY=flappyBirdY+3;					//displays bird wing
    if(y>=wingY && y<wingY+wingHeight){
      int offset=(y-wingY)*wingWidth;
      for(int i=0;i<wingWidth;i++){
        unsigned char color;
        if(wingPos==0)
          color=wingUp[offset+i];
        if(wingPos==1)
          color=wingMid[offset+i];
        if(wingPos==2)
          color=wingDown[offset+i];
        if(color!=ALPHA)
          lineBuffer[16+flappyBirdX+i]=color;
      }
    }
    display.writeBuffer(lineBuffer+16,96);
  }
  for(int z=0;z<21;z++){				//grass movement
    lineBuffer[16+(z*5)]=GREEN;
    lineBuffer[17+(z*5)]=GREEN;
    lineBuffer[18+(z*5)]=GREEN;
    lineBuffer[19+(z*5)]=DGREEN;
    lineBuffer[20+(z*5)]=DGREEN;
  }
  display.writeBuffer(lineBuffer+16+groundOffsetX,96);
  display.writeBuffer(lineBuffer+17+groundOffsetX,96);
  display.writeBuffer(lineBuffer+18+groundOffsetX,96);
  display.writeBuffer(lineBuffer+19+groundOffsetX,96);
  display.writeBuffer(lineBuffer+20+groundOffsetX,96);
  
  
  for(unsigned char i=16;i<112;i++)		//Ground
    lineBuffer[i]=BROWN;
  for(unsigned char i=0;i<7;i++)
    display.writeBuffer(lineBuffer+16,96);
  
  display.endTransfer();
}

void restartGame(void) {
  // Reset game variables
  score = 0;
  flappyBirdY = 20; // Set initial position of the bird
  pipeOffsetX = 128; // Reset pipe position
  framecount = 0;
  passedThrough = false;
}


void game_Over(int score) {
    // flag for updating the display
  boolean updateDisplay = true;

  // tracks when entered top of loop
  uint32_t loopStartMs;

  //tracks button states
  boolean btn_Accept_WasDown = false;
  boolean btn_Cancel_WasDown = false;

  // inner loop
  while (true) {
    loopStartMs = millis();

    client.loop();

    // print the display
    if (updateDisplay) {
      // clear the update flag
      updateDisplay = false;
      // clear the display
      clearScreen();

      // start the display
      display.begin();
      display.setBrightness(10);  // Set brightness level (0-100)

      // print arrow buttons
      printBtnArrows();

      // menu title
      display.setCursor(24, 24);
      display.print("GAME OVER");

      display.setCursor(24, 34);
      display.print("Score: "); display.print(score);

      display.setCursor(24, 44);
      display.print("<- retry");

    }


    // capture button down states
    if (btnIsDown(BTN_ACCEPT)) { btn_Accept_WasDown = true; }
    if (btnIsDown(BTN_CANCEL)) { btn_Cancel_WasDown = true; }

    // move the pointer up
    if (btn_Accept_WasDown && btnIsUp(BTN_ACCEPT)) {
      restartGame();
      currPage = GAME_SCREEN; return;
      btn_Accept_WasDown = false;
    }

    // move to the root menu
    if (btn_Cancel_WasDown && btnIsUp(BTN_CANCEL)) {
      currPage = SUB_MENU3;
      return;
    }

    // keep a specific pace
    while (millis() - loopStartMs < 25) { delay(2); }
  }
}
void page_Game(void) {
  
  while (true) {
  updateDisplay();
  
  display.setCursor(0, 0);
  display.print(score);

    // Check if the bird has passed between the pillars
    if (pipeOffsetX < 18 && pipeOffsetX > 6) {
      if (flappyBirdY > PipeHeight[1] && flappyBirdY + flappyBirdHeight < PipeHeight[1] + pipeSpacingY) {
        // Bird has passed between the pillars, increment the score
        if (!passedThrough) {
          score++;
          passedThrough = true;
          // Optionally, you can do something here when the bird passes through
          SerialMonitorInterface.println("Score: " + String(score));
        }
      } else {
        passedThrough = false;
      }

      // Check for collision with the pillars
      if (flappyBirdY < PipeHeight[1] || flappyBirdY + flappyBirdHeight > PipeHeight[1] + pipeSpacingY) {
        pipeColor = RED;
        game_Over(score);
        break;
      }
    } else {
      passedThrough = false;
      pipeColor = GREEN;
    }

  framecount++;
  
  if(display.getButtons()){ 		//fast wing flap, move bird up
    wingPos=(framecount)%3;
    flappyBirdY-=(birdYmod*2);
    if(flappyBirdY<0)
      flappyBirdY=0;

  }else{							// normal flap, move bird down

    wingPos=(framecount>>2)%3;
    flappyBirdY+=birdYmod;
    if(flappyBirdY>40)
      flappyBirdY=40;
  }
    
  pipeOffsetX-=1;
  
  if(movePipe && movePipe<5){ 		//moves pipe by pipe width(roughly)
    PipeHeight[movePipe-1]+=movePipeMod;
    if(PipeHeight[movePipe-1]<1)
      movePipeMod=abs(movePipeMod);
    if(PipeHeight[movePipe-1]>groundY-3-pipeSpacingY)
      movePipeMod=-1*abs(movePipeMod);
  }
  if(pipeOffsetX<=0){			 	//smooths out pipe movement
    pipeOffsetX=(pipeSpacingX+pipeWidth);
    PipeHeight[0]=PipeHeight[1];
    PipeHeight[1]=PipeHeight[2];
    PipeHeight[2]=PipeHeight[3];
    PipeHeight[3]=PipeHeight[4];
    PipeHeight[4]=3+micros()%(groundY-8-pipeSpacingY);
    if(movePipe)
      movePipe-=1;
    else
      movePipe=3+micros()%5;
  }
  
  groundOffsetX+=1;
  if(groundOffsetX>=5)
    groundOffsetX=0;
  
  if(!(framecount%2))   	//moves clouds
    cloudOffsetX--;
  if(cloudOffsetX<=0){
    cloudOffsetX=cloudSpacingX+cloudWidth;
    cloudY[0]=cloudY[1];
    cloudY[1]=cloudY[2];
    cloudY[2]=cloudY[3];
    cloudY[3]=cloudY[4];
    cloudY[4]=cloudY[5];
    cloudY[5]=micros()%(30);
  }

  /* To go back to menu */
  boolean btn_Cancel_WasDown = false;
  if (btnIsDown(BTN_CANCEL)) { btn_Cancel_WasDown = true; }

  // move to the root menu
  if (btn_Cancel_WasDown && btnIsUp(BTN_CANCEL)) {
    currPage = SUB_MENU3;
    return;
  }

  delay(25);
  }
}



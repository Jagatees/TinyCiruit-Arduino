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

  int counter = 0;
  // selected item pointer
  uint8_t sub_Pos = 1;

  // stores previous and current strings
  char weather_result[64] = "";

  // inner loop
  while (true) {
    
    loopStartMs = millis();

    // if (clietn.client() != "") {
    //   weather_result = loop.client();
    //   updateDynamicSection = true;
    // } else {
    //   weather_result = "No Data yet";
    //   updateDynamicSection = true;
    // }

    client.loop();
    //counter++;
    updateDynamicSection = true;

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
      display.print("[ WEATHER SCREEN ]");

    }

    // update only a section of the screen
    if (updateDynamicSection) {
      updateDynamicSection = false;
      // call the weather function and get the returned string
      // print the items
      display.setCursor(24, 32); // centered to the middle of screen
      //display.print("Counter: ");
      //display.print(counter);

      display.print(receivedPayload);

    }
    // capture button down states
    if (btnIsDown(BTN_UP)) {btn_Up_WasDown = true;}
    if (btnIsDown(BTN_DOWN)) {btn_Down_WasDown = true;}
    if (btnIsDown(BTN_CANCEL)) {btn_Cancel_WasDown = true;}


    // move the pointer down
    if (btn_Down_WasDown && btnIsUp(BTN_DOWN)) {
      if (sub_Pos == SUB_MENU3_COUNT) { sub_Pos = 1; } else { sub_Pos++; }
      updateDynamicSection = true;
      btn_Down_WasDown = false;
    } 

    // move the pointer up
    if (btn_Up_WasDown && btnIsUp(BTN_UP)) {
      if (sub_Pos == 1) { sub_Pos = SUB_MENU3_COUNT; } else { sub_Pos--; }
      updateDynamicSection = true;
      btn_Up_WasDown = false;
    } 

    // move to the root menu
    if (btn_Cancel_WasDown && btnIsUp(BTN_CANCEL)) { currPage = ROOT_MENU; return; }
    // keep a specific pace
    while (millis() - loopStartMs < 25) { delay(2); }
  }
}

// =========================================================================
// ||                          PAGE - TEST PAGE                           || 
// =========================================================================  
void page_Test(void) {
  // flag for updating the display
  boolean updateDisplay = true;
  boolean updateDynamicSection = true;

  int counter = 0;
  // tracks when entered top of loop
  uint32_t loopStartMs;

  //tracks button states
  boolean btn_Up_WasDown = false;
  boolean btn_Down_WasDown = false;
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
      display.print("[ TEST COUNTER ]");
    }

    if (updateDynamicSection) {
      updateDynamicSection = false;
      // call the weather function and get the returned string
      // print the items
      display.setCursor(24, 32); 
      display.print("Counter: ");
      display.print(counter);

    }
    // capture button down states
    if (btnIsDown(BTN_UP)) {btn_Up_WasDown = true;}
    if (btnIsDown(BTN_DOWN)) {btn_Down_WasDown = true;}
    if (btnIsDown(BTN_CANCEL)) {btn_Cancel_WasDown = true;}


    // move the pointer down
    if (btn_Down_WasDown && btnIsUp(BTN_DOWN)) {
      if (counter > 0) {
        counter--;
        updateDynamicSection = true;
        btn_Down_WasDown = false;
      } 
    } 

    // move the pointer up
    if (btn_Up_WasDown && btnIsUp(BTN_UP)) {
      counter++;
      updateDynamicSection = true;
      btn_Up_WasDown = false;
      
    } 

    // move to the root menu
    if (btn_Cancel_WasDown && btnIsUp(BTN_CANCEL)) { currPage = ROOT_MENU; return; }

    SerialMonitorInterface.print(counter);
    // keep a specific pace
    while (millis() - loopStartMs < 25) { delay(2); }
  }
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

  // selected item pointer
  uint8_t sub_Pos = 1;

  String result = dictionary.get("HootHoot/Start");
  


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
      display.print("[ HOOTHOOT START ]");
    }

    if (updateDynamicSection) {
      updateDynamicSection = false;
      // call the weather function and get the returned string
      // print the items
      display.setCursor(24, 32); 
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
    if (btnIsDown(BTN_UP)) {btn_Up_WasDown = true;}
    if (btnIsDown(BTN_DOWN)) {btn_Down_WasDown = true;}
    if (btnIsDown(BTN_ACCEPT)) {btn_Accept_WasDown = true;}

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
    if (btn_Accept_WasDown && btnIsUp(BTN_ACCEPT)) { if (readyToStartQuiz) { currPage = HOOTHOOT_QUIZ_SCREEN; } return; }

    // keep a specific pace
    while (millis() - loopStartMs < 25) { delay(2); }
  }
}

// =========================================================================
// ||                     PAGE - HOOTHOOT QUIZ PAGE                      || 
// =========================================================================  
void page_HootHootQuiz(void) {
  // flag for updating the display
  boolean updateDisplay = true;
  boolean updateDynamicSection = true;
  boolean endQuiz = false;
  
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
  String user_input;

  //int centerX = (display.width() - (display.textWidth(result1 + result2))) / 2;

  // inner loop
  while (true) {
    loopStartMs = millis();

    
    if(dictionary.get("HootHoot/Question1/Option1") == "A") {
      result1 = dictionary.get("HootHoot/Question1/Option1");
    } 

    if(dictionary.get("HootHoot/Question1/Option2") == "B") {
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
      display.setBrightness(10); // Set brightness level (0-100)

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
        display.print("You have chosen: " + user_input);
      }
      display.setCursor(24, 45);  
      display.print(result1);

      display.setCursor(64, 45); 
      display.print(result2);

    }
    // capture button down states
    if (btnIsDown(BTN_DOWN)) {btn_Down_WasDown = true;}
    if (btnIsDown(BTN_UP)) {btn_Up_WasDown = true;}

    
    // move the pointer up
    if (btn_Down_WasDown && btnIsUp(BTN_DOWN) && !btn_Down_Disabled) {
      user_input = result2;
      updateDynamicSection = true;
      btn_Down_WasDown = false;
      btn_Down_Disabled = true;
    } 
    // move to the root menu
    if (btn_Up_WasDown && btnIsUp(BTN_UP) && !btn_Up_Disabled) { 
      user_input = result1;
      updateDynamicSection = true;
      btn_Up_WasDown = false;
      btn_Up_Disabled = true;
    }

    // keep a specific pace
    while (millis() - loopStartMs < 25) { delay(2); }
  }
}
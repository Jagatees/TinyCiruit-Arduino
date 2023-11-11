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
      client.publish("Vanessa","here");
      name = "Vanessa";
      display.setCursor(0, 42);
      display.print("Attendance taken!");

      updateDynamicSection = true;
      btn_Up_WasDown = false;
      
    } 

    // move to the root menu
    if (btn_Cancel_WasDown && btnIsUp(BTN_CANCEL)) { currPage = ROOT_MENU; return; }

    // keep a specific pace
    while (millis() - loopStartMs < 25) { delay(2); }
  }
}

==================== MAIN PAGE =================
enum pageType { LOCK_SCREEN, ROOT_MENU, SUB_MENU1, SUB_MENU2, SUB_MENU3, WEATHER_SCREEN, TEST_SCREEN, GRAPH_SCREEN, HEART_SCREEN, HOOTHOOT_START_SCREEN, HOOTHOOT_QUIZ_SCREEN, ATTENDANCE_SCREEN};


// =========================================================================
// ||                          PAGE - SUB MENU2                           || 
// =========================================================================  
void page_SubMenu2(void) {
    // flag for updating the display
  boolean updateDisplay = true;

  // tracks when entered top of loop
  uint32_t loopStartMs;

  //tracks button states
  boolean btn_Up_WasDown = false;
  boolean btn_Down_WasDown = false;
  boolean btn_Cancel_WasDown = false;
  boolean btn_Accept_WasDown = false;

  // selected item pointer
  uint8_t sub_Pos = 1;
  // inner loop
  while (true) {
    loopStartMs = millis();

    // print the display
    if (updateDisplay) {
    
      // clear the update flag
      updateDisplay = false;

      // print the items
      //Serial.println(F("First Selectable Item"));
      //Serial.println(F("Second Selectable Item"));
      //Serial.println(F("Third Selectable Item"));
      //Serial.println(F("Fourth Selectable Item"));

      // clear the display
      clearScreen();

      // start the display
      display.begin();
      display.setBrightness(10); // Set brightness level (0-100)

      // print arrow buttons
      printBtnArrows();

      // menu title
      display.setCursor(0, 0); 
      display.print("[ SUB MENU #2 ]");

      // print a divider line
      printDivider();

      // print the items
      display.setCursor(0, 10); 
      printSelected(1, sub_Pos); display.print("Attendance");
      
      display.setCursor(0, 20);
      printSelected(2, sub_Pos); display.print("Second Item");

      display.setCursor(0, 30);
      printSelected(3, sub_Pos); display.print("Third Item");

      // print a divider line
      display.setCursor(0, 50);
      printDivider();
    }
    // capture button down states
    if (btnIsDown(BTN_UP)) {btn_Up_WasDown = true;}
    if (btnIsDown(BTN_DOWN)) {btn_Down_WasDown = true;}
    if (btnIsDown(BTN_CANCEL)) {btn_Cancel_WasDown = true;}
    if (btnIsDown(BTN_ACCEPT)) {btn_Accept_WasDown = true;}

    // move the pointer down
    if (btn_Down_WasDown && btnIsUp(BTN_DOWN)) {
      if (sub_Pos == SUB_MENU2_COUNT) { sub_Pos = 1; } else { sub_Pos++; }
      updateDisplay = true;
      btn_Down_WasDown = false;
    } 

    // move the pointer up
    if (btn_Up_WasDown && btnIsUp(BTN_UP)) {
      if (sub_Pos == 1) { sub_Pos = SUB_MENU2_COUNT; } else { sub_Pos--; }
      updateDisplay = true;
      btn_Up_WasDown = false;
    } 
    if (btn_Accept_WasDown && btnIsUp(BTN_ACCEPT)) {
      switch (sub_Pos) {
        case 1: currPage = ATTENDANCE_SCREEN; return;
      }
    }
    // move to the root menu
    if (btn_Cancel_WasDown && btnIsUp(BTN_CANCEL)) { currPage = ROOT_MENU; return; }
    // keep a specific pace
    while (millis() - loopStartMs < 25) { delay(2); }
  }
}
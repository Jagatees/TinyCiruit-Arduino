// =========================================================================
// ||                          PAGE - SUB MENU1                           ||
// =========================================================================
void page_SubMenu1(void) {
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
      display.setBrightness(10);  // Set brightness level (0-100)

      // print arrow buttons
      printBtnArrows();

      // menu title
      display.setCursor(0, 0);
      display.print("[ APIs MENU ]");

      // print a divider line
      printDivider();

      printDivider();
    }

    if (updateDynamicSection) {
      updateDynamicSection = false;
      // print the items
      display.setCursor(0, 10);
      printSelected(1, sub_Pos);
      display.print("Weather");

      display.setCursor(0, 20);
      printSelected(2, sub_Pos); display.print("Hoot Hoot");

      display.setCursor(0, 30);
      printSelected(3, sub_Pos); display.print("Open AI");

      display.setCursor(0, 40);
      printSelected(4, sub_Pos); display.print("Silent Helper");

      display.setCursor(0, 50);
      printSelected(5, sub_Pos); display.print("Telebot");

      // print a divider line
      display.setCursor(0, 50);
    }
    // capture button down states
    if (btnIsDown(BTN_UP)) { btn_Up_WasDown = true; }
    if (btnIsDown(BTN_DOWN)) { btn_Down_WasDown = true; }
    if (btnIsDown(BTN_CANCEL)) { btn_Cancel_WasDown = true; }
    if (btnIsDown(BTN_ACCEPT)) { btn_Accept_WasDown = true; }

    // move the pointer down
    if (btn_Down_WasDown && btnIsUp(BTN_DOWN)) {
      if (sub_Pos == SUB_MENU1_COUNT) {
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
        sub_Pos = SUB_MENU1_COUNT;
      } else {
        sub_Pos--;
      }
      updateDynamicSection = true;
      btn_Up_WasDown = false;
    }

    if (btn_Accept_WasDown && btnIsUp(BTN_ACCEPT)) {
      switch (sub_Pos) {
        case 1: currPage = WEATHER_SCREEN; return;
        case 2: currPage = HOOTHOOT_START_SCREEN; return;
        case 3: currPage = OPENAI_SCREEN; return;
        //case 5: currPage = TELEBOT_SCREEN; return;
        //case 5: currPage = TEST_SCREEN; return;
      }
    }
    // move to the root menu
    if (btn_Cancel_WasDown && btnIsUp(BTN_CANCEL)) {
      currPage = ROOT_MENU;
      return;
    }
    // keep a specific pace
    while (millis() - loopStartMs < 25) { delay(2); }
  }
}
// =========================================================================
// ||                          PAGE - SUB MENU2                           ||
// =========================================================================
void page_SubMenu2(void) {
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
      display.setBrightness(10);  // Set brightness level (0-100)

      // print arrow buttons
      printBtnArrows();

      // menu title
      display.setCursor(0, 0);
      display.print("[ SYSTEM APPS #2 ]");

      // print a divider line
      printDivider();

      // print a divider line
      display.setCursor(0, 50);
      printDivider();
    }

    if (updateDynamicSection) {
      updateDynamicSection = false;
      // print the items
      display.setCursor(0, 10);
      printSelected(1, sub_Pos);
      display.print("Attendance");

      display.setCursor(0, 20);
      printSelected(2, sub_Pos);
      display.print("Alarm");

      display.setCursor(0, 30);
      printSelected(3, sub_Pos);
      display.print("SILENT HELPER");

      display.setCursor(0, 40);
      printSelected(4, sub_Pos);
      display.print("Audio");

      display.setCursor(0, 50);
      printSelected(4, sub_Pos);
      display.print("Game");
    }
    // capture button down states
    if (btnIsDown(BTN_UP)) { btn_Up_WasDown = true; }
    if (btnIsDown(BTN_DOWN)) { btn_Down_WasDown = true; }
    if (btnIsDown(BTN_CANCEL)) { btn_Cancel_WasDown = true; }
    if (btnIsDown(BTN_ACCEPT)) { btn_Accept_WasDown = true; }

    // move the pointer down
    if (btn_Down_WasDown && btnIsUp(BTN_DOWN)) {
      if (sub_Pos == SUB_MENU2_COUNT) {
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
        sub_Pos = SUB_MENU2_COUNT;
      } else {
        sub_Pos--;
      }
      updateDynamicSection = true;
      btn_Up_WasDown = false;
    }

    if (btn_Accept_WasDown && btnIsUp(BTN_ACCEPT)) {
      switch (sub_Pos) {
        case 1: currPage = OXIMETER_SCREEN; return;
        case 2: currPage = ALARM_SCREEN; return;
        case 3: currPage = SILENTHELPER_SCREEN;return;
        case 4: currPage = AUDIO_SCREEN; return;
        case 5: currPage = GAME_SCREEN; return;
        case 6: currPage = ATTENDANCE_SCREEN; return;
      }
    }

    // move to the root menu
    if (btn_Cancel_WasDown && btnIsUp(BTN_CANCEL)) {
      currPage = ROOT_MENU;
      return;
    }
    // keep a specific pace
    while (millis() - loopStartMs < 25) { delay(2); }
  }
}
// =========================================================================
// ||                          PAGE - SUB MENU3                           ||
// =========================================================================
void page_SubMenu3(void) {
  // flag for updating the display
  boolean updateDisplay = true;

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
      display.setBrightness(10);  // Set brightness level (0-100)

      // print arrow buttons
      printBtnArrows();

      // menu title
      display.setCursor(0, 0);
      display.print("[ SUB MENU #1 ]");

      // print a divider line
      printDivider();

      // print the items
      display.setCursor(0, 10);
      printSelected(1, sub_Pos);
      display.print("First Item");

      display.setCursor(0, 20);
      printSelected(2, sub_Pos);
      display.print("Second Item");

      display.setCursor(0, 30);
      printSelected(3, sub_Pos);
      display.print("Third Item");

      // print a divider line
      display.setCursor(0, 50);
      printDivider();
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
      updateDisplay = true;
      btn_Down_WasDown = false;
    }

    // move the pointer up
    if (btn_Up_WasDown && btnIsUp(BTN_UP)) {
      if (sub_Pos == 1) {
        sub_Pos = SUB_MENU3_COUNT;
      } else {
        sub_Pos--;
      }
      updateDisplay = true;
      btn_Up_WasDown = false;
    }

    // move to the root menu
    if (btn_Cancel_WasDown && btnIsUp(BTN_CANCEL)) {
      currPage = ROOT_MENU;
      return;
    }
    // keep a specific pace
    while (millis() - loopStartMs < 25) { delay(2); }
  }
}
#include <TimeLib.h>
#include <Wire.h>
#include <TinyScreen.h>
#include <GraphicsBuffer.h>
#include <SPI.h>
#include <WiFi101.h>
#include <PubSubClient.h>

#define ROOT_MENU_COUNT 3
#define SUB_MENU1_COUNT 4
#define SUB_MENU2_COUNT 5
#define SUB_MENU3_COUNT 2
#define LOCK_SCREEN_DURATION 10000 // 60 secs
#define BLACK 0x0000 // For 16-bit color displays

TinyScreen display = TinyScreen(0);
GraphicsBuffer displayBuffer = GraphicsBuffer(96, 64, colorDepth1BPP);

// use enum to make things more readable and flexible
// setup the enum with all the menu page options
enum pageType { LOCK_SCREEN, ROOT_MENU, SUB_MENU1, SUB_MENU2, SUB_MENU3, WEATHER_SCREEN };

// holds which page is currently selected
enum  pageType currPage = LOCK_SCREEN;

// selected item pointer for the root menu
uint8_t root_Pos = 1;

// constants holding prot addresses
const int BTN_ACCEPT = TSButtonLowerLeft;
const int BTN_UP = TSButtonUpperRight;
const int BTN_DOWN = TSButtonLowerRight;
const int BTN_CANCEL = TSButtonUpperLeft;

// track timer when leaving the lock screen
unsigned long main_menu_start = 0;

// Make Serial Monitor compatible for all TinyCircuits processors
#if defined(ARDUINO_ARCH_AVR)
  #define SerialMonitorInterface Serial
#elif defined(ARDUINO_ARCH_SAMD)
  #define SerialMonitorInterface SerialUSB
#endif

// WIFI & MQTT VARIABLES 

const char* ssid = "SINGTEL-C8NA";
const char* wifiPassword = "57hhcumfd8";
// Create an instance of WiFiClient
WiFiClient espClient;

// MQTT broker details
const char* mqttServer = "192.168.1.83";
const int mqttPort = 1883;

// Create an instance of PubSubClient
PubSubClient client;

#if defined(ARDUINO_ARCH_SAMD)
  #define SerialMonitorInterface SerialUSB
#else
  #define SerialMonitorInterface Serial
#endif

// ========================================================================
// ||                             SETUP                                 || 
// ========================================================================         
void setup() {
  // put your setup code here, to run once:
  initWiFi();
  initMQTT();
  // init the serial port to be used as a display return
  Wire.begin();
  SerialMonitorInterface.begin(9600);
  display.begin();
  display.setFont(thinPixel7_10ptFontInfo);
  display.setBrightness(10);
  display.setFlip(true);
}
// ========================================================================
// ||                             MAIN LOOP                              || 
// ========================================================================   
void loop() {


  client.loop();

  /*if (btnIsDown(TSButtonLowerLeft)) { SerialMonitorInterface.println("ACCEPT"); } 

  if (btnIsDown(TSButtonUpperLeft)) { SerialMonitorInterface.println("CANCEL");} 

  if (btnIsDown(TSButtonUpperRight)) { SerialMonitorInterface.println("UP");} 

  if (btnIsDown(TSButtonLowerRight)) {  SerialMonitorInterface.println("DOWN");} */
  // put your main code here, to run repeatedly:

  // Print the currPage variable
  SerialMonitorInterface.print("Current Page: ");
  SerialMonitorInterface.print(millis());

  
  if (currPage == LOCK_SCREEN) {

    // start tracking the milliseconds
    page_LockScreen();
    
  } else {
    switch(currPage) {
      case LOCK_SCREEN: page_LockScreen(); break;
      case ROOT_MENU: page_RootMenu(); break;
      case SUB_MENU1: page_SubMenu1(); break;
      case SUB_MENU2: page_SubMenu2(); break;
      case SUB_MENU3: page_SubMenu3(); break;
      case WEATHER_SCREEN: page_Weather(); break;
    }

  }
}

// =========================================================================
// ||                          PAGE - LOCK SCREEN                          || 
// =========================================================================  

void page_LockScreen(void) {
  // flag for updating the display
  boolean updateDisplay = true;

  // tracks when entered top of loop
  uint32_t loopStartMs;

  //tracks button states
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
      display.setBrightness(10); // Set brightness level (0-100)

      // print arrow buttons
      printBtnArrows();

      //print date time for lockscreen
      printDateTime();
      
    }
    // capture button down states
    if (btnIsDown(BTN_ACCEPT)) {btn_Accept_WasDown = true;}

    // move to the root menu
    if (btn_Accept_WasDown && btnIsUp(BTN_ACCEPT)) { currPage = ROOT_MENU; return; }
    // keep a specific pace
    while (millis() - loopStartMs < 25) { delay(2); }
  }

}
// =========================================================================
// ||                          PAGE - ROOT MENU                           || 
// =========================================================================  
void page_RootMenu(void) {

  // flag for updating the display
  boolean updateDisplay = true;

  // tracks when entered top of loop
  uint32_t loopStartMs;

  //tracks button states
  boolean btn_Up_WasDown = false;
  boolean btn_Down_WasDown = false;
  boolean btn_Accept_WasDown = false;


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
      display.print("[ MAIN MENU ]");

      // print a divider line
      printDivider();

      // print the items
      display.setCursor(0, 10); 
      printSelected(1, root_Pos); display.print("Sub Menu One");
      
      display.setCursor(0, 20);
      printSelected(2, root_Pos); display.print("Sub Menu Two");

      display.setCursor(0, 30);
      printSelected(3, root_Pos); display.print("Sub Menu Three");

      // print a divider line
      display.setCursor(0, 50);
      printDivider();
    }
    // capture button down states
    if (btnIsDown(BTN_UP)) {btn_Up_WasDown = true;}
    if (btnIsDown(BTN_DOWN)) {btn_Down_WasDown = true;}
    if (btnIsDown(BTN_ACCEPT)) {btn_Accept_WasDown = true;}


    // move the pointer down
    if (btn_Down_WasDown && btnIsUp(BTN_DOWN)) {
      if (root_Pos == ROOT_MENU_COUNT) { root_Pos = 1; } else { root_Pos++; }
      updateDisplay = true;
      btn_Down_WasDown = false;
    } 

    // move the pointer up
    if (btn_Up_WasDown && btnIsUp(BTN_UP)) {
      if (root_Pos == 1) { root_Pos = ROOT_MENU_COUNT; } else { root_Pos--; }
      updateDisplay = true;
      btn_Up_WasDown = false;
    } 

    if (btn_Accept_WasDown && btnIsUp(BTN_ACCEPT)) {
      switch (root_Pos) {
        case 1: currPage = SUB_MENU1; return;
        case 2: currPage = SUB_MENU2; return;
        case 3: currPage = SUB_MENU3; return;
      }
    }
    // keep a specific pace
    while (millis() - loopStartMs < 25) { delay(2); }
  }
}
// =========================================================================
// ||                          PAGE - SUB MENU1                           || 
// =========================================================================  
void page_SubMenu1(void) {
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
      display.print("[ APIs MENU ]");

      // print a divider line
      printDivider();

      // print the items
      display.setCursor(0, 10); 
      printSelected(1, sub_Pos); display.print("Get Weather");
      
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
      if (sub_Pos == SUB_MENU1_COUNT) { sub_Pos = 1; } else { sub_Pos++; }
      updateDisplay = true;
      btn_Down_WasDown = false;
    } 

    // move the pointer up
    if (btn_Up_WasDown && btnIsUp(BTN_UP)) {
      if (sub_Pos == 1) { sub_Pos = SUB_MENU1_COUNT; } else { sub_Pos--; }
      updateDisplay = true;
      btn_Up_WasDown = false;
    } 

    if (btn_Accept_WasDown && btnIsUp(BTN_ACCEPT)) {
      switch (sub_Pos) {
        case 1: currPage = WEATHER_SCREEN; return;
      }
    }
    // move to the root menu
    if (btn_Cancel_WasDown && btnIsUp(BTN_CANCEL)) { currPage = ROOT_MENU; return; }
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
      printSelected(1, sub_Pos); display.print("First Item");
      
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

    // move to the root menu
    if (btn_Cancel_WasDown && btnIsUp(BTN_CANCEL)) { currPage = ROOT_MENU; return; }
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
      display.print("[ SUB MENU #1 ]");

      // print a divider line
      printDivider();

      // print the items
      display.setCursor(0, 10); 
      printSelected(1, sub_Pos); display.print("First Item");
      
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


    // move the pointer down
    if (btn_Down_WasDown && btnIsUp(BTN_DOWN)) {
      if (sub_Pos == SUB_MENU3_COUNT) { sub_Pos = 1; } else { sub_Pos++; }
      updateDisplay = true;
      btn_Down_WasDown = false;
    } 

    // move the pointer up
    if (btn_Up_WasDown && btnIsUp(BTN_UP)) {
      if (sub_Pos == 1) { sub_Pos = SUB_MENU3_COUNT; } else { sub_Pos--; }
      updateDisplay = true;
      btn_Up_WasDown = false;
    } 

    // move to the root menu
    if (btn_Cancel_WasDown && btnIsUp(BTN_CANCEL)) { currPage = ROOT_MENU; return; }
    // keep a specific pace
    while (millis() - loopStartMs < 25) { delay(2); }
  }
}

// =========================================================================
// ||                          PAGE - WEATHER SCREEN                      || 
// =========================================================================  
void page_Weather(void) {
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
      display.setBrightness(10); // Set brightness level (0-100)

      // print arrow buttons
      printBtnArrows();

      // menu title
      display.setCursor(0, 0); 
      display.print("[ WEATHER SCREEN ]");

      // print a divider line
      printDivider();

      // call the weather function and get the returned string
      // print the items
      display.setCursor(0, 10); 
      char weather_result = getWeather();

      display.print(weather_result);

      // print a divider line
      display.setCursor(0, 50);
      printDivider();
    }
    // capture button down states
    if (btnIsDown(BTN_UP)) {btn_Up_WasDown = true;}
    if (btnIsDown(BTN_DOWN)) {btn_Down_WasDown = true;}
    if (btnIsDown(BTN_CANCEL)) {btn_Cancel_WasDown = true;}


    // move the pointer down
    if (btn_Down_WasDown && btnIsUp(BTN_DOWN)) {
      if (sub_Pos == SUB_MENU3_COUNT) { sub_Pos = 1; } else { sub_Pos++; }
      updateDisplay = true;
      btn_Down_WasDown = false;
    } 

    // move the pointer up
    if (btn_Up_WasDown && btnIsUp(BTN_UP)) {
      if (sub_Pos == 1) { sub_Pos = SUB_MENU3_COUNT; } else { sub_Pos--; }
      updateDisplay = true;
      btn_Up_WasDown = false;
    } 

    // move to the root menu
    if (btn_Cancel_WasDown && btnIsUp(BTN_CANCEL)) { currPage = ROOT_MENU; return; }
    // keep a specific pace
    while (millis() - loopStartMs < 25) { delay(2); }
  }
}

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
  for (uint8_t i = 0; i < 40; i++) { display.print("-");}
}

void printDateTime(void) {
  display.clearWindow(20, 45, 50, 16);
  //display.setFont(liberationSansNarrow_12ptFontInfo);   //Set the font type

  // Print date in US format MM:DD:YY (Switch the order in which day, month, year that you like to use)
  display.setCursor(15,8); //Set the cursor where you want to start printing the date
  display.print(monthShortStr(month()));
  display.print(" ");
  display.print(day()); 
  display.print(", ");
  display.print(year());

  display.setCursor(30,25); //Set the cursor where you want to start printing the date  
  display.print(dayStr(weekday()));

  //display.setFont(liberationSansNarrow_16ptFontInfo);   //Set the font type

  // display time in HH:MM:SS 24 hour format
  display.setCursor(20,45); //Set the cursor where you want to start printing the time
  if(hour()<10) display.print(0); //print a leading 0 if hour value is less than 0
  display.print(hour());
  display.print(":");
  if(minute()<10) display.print(0); //print a leading 0 if minute value is less than 0
  display.print(minute());
  display.print(":");
  if(second()<10) display.print(0); //print a leading 0 if seconds value is less than 0
  display.print(second());
  display.print(" "); //just a empty space after the seconds
  //delay(1000); //delay 60 second
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
// ||                          FUNCTION - WIFI                            || 
// ========================================================================= 

void initWiFi() {
    SerialMonitorInterface.begin(9600);
    while (!SerialMonitorInterface); // Wait until serial is ready

    SerialMonitorInterface.println("Starting WiFi initialization...");

    WiFi.setPins(8, 2, A3, -1); // VERY IMPORTANT FOR TINYDUINO

    // Attempt to connect to WiFi
    SerialMonitorInterface.print("Attempting to connect to WiFi SSID: ");
    SerialMonitorInterface.println(ssid);

    WiFi.begin(ssid, wifiPassword);
    while (WiFi.status() != WL_CONNECTED) {
        SerialMonitorInterface.print(".");
        delay(500);
    }

    // Print connection details
    SerialMonitorInterface.println("");
    SerialMonitorInterface.println("Successfully connected to WiFi!");
    IPAddress ip = WiFi.localIP();
    SerialMonitorInterface.print("Assigned IP address: ");
    SerialMonitorInterface.println(ip);
}

// =========================================================================
// ||                          FUNCTION - MQTT                            || 
// ========================================================================= 

void initMQTT() {
    SerialMonitorInterface.println("Setting up MQTT connection...");

    client.setClient(espClient);
    // Set MQTT broker details
    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);

    // Connect to MQTT broker
    while (!client.connected()) {
        SerialMonitorInterface.println("Attempting to connect to MQTT broker...");
        if (client.connect("mqtt-ardunio-derpee")) {
            SerialMonitorInterface.println("Successfully connected to MQTT broker!");
            break;
        }

        SerialMonitorInterface.println(client.state());
        SerialMonitorInterface.println("Failed to connect to MQTT broker. Retrying in 5 seconds...");
        delay(5000);
    }

    // Subscribe to a topic
    client.subscribe("Request/API"); 
    SerialMonitorInterface.println("Request/API");
}

void callback(char* topic, byte* payload, unsigned int length) {
    SerialMonitorInterface.println("Callback invoked!");
    SerialMonitorInterface.print("Received message on topic '");
    SerialMonitorInterface.print(topic);
    SerialMonitorInterface.print("': ");
    for (int i = 0; i < length; i++) {
        SerialMonitorInterface.print((char)payload[i]);
    }
    SerialMonitorInterface.println();
}


// =========================================================================
// ||                          FUNCTION - WIFI                            || 
// ========================================================================= 
int getWeather(void) {



  return 0;
  // enter code here
}

#include <TimeLib.h>
#include <Wire.h>
#include <TinyScreen.h>
#include <GraphicsBuffer.h>
#include <SPI.h>

// WIFI & MQTT
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
enum pageType { LOCK_SCREEN, ROOT_MENU, SUB_MENU1, SUB_MENU2, SUB_MENU3, WEATHER_SCREEN, TEST_SCREEN };

// holds which page is currently selected
enum  pageType currPage = WEATHER_SCREEN;

enum State { MAIN_MENU, MQTT };

enum State currState = MAIN_MENU;

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

char receivedPayload[256]; // Define a global char array to store the payload
int payloadLength = 0; // Global variable to store the length of the payload

#if defined(ARDUINO_ARCH_SAMD)
  #define SerialMonitorInterface SerialUSB
#else
  #define SerialMonitorInterface Serial
#endif

// ========================================================================
// ||                             SETUP                                 || 
// ========================================================================         
void setup() {
  
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


  // This new to be running as often as possiable 

  /*if (btnIsDown(TSButtonLowerLeft)) { SerialMonitorInterface.println("ACCEPT"); } 

  if (btnIsDown(TSButtonUpperLeft)) { SerialMonitorInterface.println("CANCEL");} 

  if (btnIsDown(TSButtonUpperRight)) { SerialMonitorInterface.println("UP");} 

  if (btnIsDown(TSButtonLowerRight)) {  SerialMonitorInterface.println("DOWN");} */
  // put your main code here, to run repeatedly:

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
        case TEST_SCREEN: page_Test(); break;
      }
    /*switch(currState) {
      case MAIN_MENU:
        switch(currPage) {
          case LOCK_SCREEN: page_LockScreen(); break;
          case ROOT_MENU: page_RootMenu(); break;
          case SUB_MENU1: page_SubMenu1(); break;
          case SUB_MENU2: page_SubMenu2(); break;
          case SUB_MENU3: page_SubMenu3(); break;
          case WEATHER_SCREEN: page_Weather(); break;
          case TEST_SCREEN: page_Test(); break;
        }
        break;
      case MQTT:
        //client.loop();
        break;
      default:
        break;
    }*/

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
      printSelected(1, root_Pos); display.print("APIs Menu");
      
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
    
    // Copy the payload into the global variable
    payloadLength = length;
    for (int i = 0; i < length; i++) {
        receivedPayload[i] = (char)payload[i];
    }
    
    // Null-terminate the global variable
    receivedPayload[length] = '\0';

    // Print the received payload
    SerialMonitorInterface.println(receivedPayload);
}





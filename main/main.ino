/* General Libraries*/
#include <SPI.h>
#include <Wire.h>
#include <stdio.h>

/* Screen Libraries*/
#include <TinyScreen.h>
#include <GraphicsBuffer.h>

/* WIFI & MQTT */
#include <WiFi101.h>
#include <PubSubClient.h>

/* Audio Libraries */
#include <SD.h>
#include <AudioZero.h>

/* Clock Libraries */
#include <TimeLib.h>
#include <RTCZero.h>

#include <Wireling.h> // Interface with Wirelings
#include "pitches.h"

#include <MAX30101.h>

/* Menu Definitions */
#define ROOT_MENU_COUNT 3
#define SUB_MENU1_COUNT 6
#define SUB_MENU2_COUNT 3
#define SUB_MENU3_COUNT 2
#define LOCK_SCREEN_DURATION 10000  // 60 secs
#define BLACK 0x0000                // For 16-bit color displays

TinyScreen display = TinyScreen(0);
GraphicsBuffer displayBuffer = GraphicsBuffer(96, 64, colorDepth1BPP);

// use enum to make things more readable and flexible
// setup the enum with all the menu page options
enum pageType {
  // Main
  LOCK_SCREEN,
  ROOT_MENU,
  SUB_MENU1,
  SUB_MENU2,
  SUB_MENU3,
  // Education
  HOOTHOOT_START_SCREEN,
  HOOTHOOT_QUIZ_SCREEN,
  HOOTHOOT_SUBMISSION_SCREEN,
  ATTENDANCE_SCREEN,
  SILENTHELPER_SCREEN,
  TELEBOT_SCREEN,
  GOOGLE_SCREEN,
  SMS_SCREEN,
  // System
  WEATHER_SCREEN,
  ALARM_SCREEN,
  OXIMETER_SCREEN,
  // Entertainment
  AUDIO_SCREEN,
  GAME_SCREEN
};

// holds which page is currently selected
enum pageType currPage = ROOT_MENU;

// selected item pointer for the root menu
uint8_t root_Pos = 1;

// constants holding prot addresses
const int BTN_ACCEPT = TSButtonLowerLeft;
const int BTN_UP = TSButtonUpperRight;
const int BTN_DOWN = TSButtonLowerRight;
const int BTN_CANCEL = TSButtonUpperLeft;

// track timer when leaving the lock screen
unsigned long main_menu_start = 0;

/* RTC */
RTCZero rtc;

// Make Serial Monitor compatible for all TinyCircuits processors
#if defined(ARDUINO_ARCH_AVR)
#define SerialMonitorInterface Serial
#elif defined(ARDUINO_ARCH_SAMD)
#define SerialMonitorInterface SerialUSB
#endif


MAX30101 pulseSensor = MAX30101();
int pulseSensorPort = 2;

// =========================================================================
// ||                          VARIABLES - WIFI & MQTT                      ||
// =========================================================================

const char* ssid = "SINGTEL-C8NA";
const char* wifiPassword = "57hhcumfd8";

// Create an instance of WiFiClient
WiFiClient espClient;

// MQTT broker details
const char* mqttServer = "192.168.1.83";
const int mqttPort = 1883;

// Create an instance of PubSubClient
PubSubClient client;

char receivedPayload[256];  // Define a global char array to store the payload
int payloadLength = 0;      // Global variable to store the length of the payload

// =========================================================================
// ||                          VARIABLES - RTC                            ||
// =========================================================================
// We'll dynamically change these values to set the current initial time
// No need to change them here.
byte seconds = 0;
byte minutes = 45;
byte hours = 9;

// We'll dynamically change these values to set the current initial date
// The preset values are only examples.
byte days = 13;
byte months = 3;
byte years = 16;

// declare alarm variables
int alarmHour = 0;
int alarmMinute = 0;
bool alarmSet = false;

int clock_brightness = 15;


// =========================================================================
// ||                          c++ - dictionary                           ||
// ==================================================cl=======================
const int MAX_SIZE = 256;  // Adjust this as needed.

struct KeyValuePair {
  String key;
  String value;
};

class SimpleDictionary {
private:
  KeyValuePair pairs[MAX_SIZE];
  int size;

public:
  SimpleDictionary()
    : size(0) {}

  // Check if a key exists in the dictionary and return its index, or -1 if not found
  int keyIndex(const String& key) {
    for (int i = 0; i < size; i++) {
      if (pairs[i].key == key) {
        return i;  // Key found, return its index.
      }
    }
    return -1;  // Key not found.
  }

  // Add or update a key-value pair
  bool add(const String& key, const String& value) {
    int index = keyIndex(key);

    if (index != -1) {
      // Key already exists, update the value.
      pairs[index].value = value;
      return true;
    } else if (size < MAX_SIZE) {
      // Key doesn't exist, and there's space in the dictionary.
      pairs[size].key = key;
      pairs[size].value = value;
      size++;
      return true;
    }
    return false;  // Dictionary is full.
  }
  // Get a value by its key
  String get(const String& key) {
    for (int i = 0; i < size; i++) {
      if (pairs[i].key == key) {
        return pairs[i].value;
      }
    }
    return "";  // Not found.
  }

  // Print the entire dictionary
  void printAll() {
    for (int i = 0; i < size; i++) {
      SerialMonitorInterface.println(pairs[i].key + ": " + pairs[i].value);
    }
  }
};

SimpleDictionary dictionary;

#if defined(ARDUINO_ARCH_SAMD)
#define SerialMonitorInterface SerialUSB
#else
#define SerialMonitorInterface Serial
#endif

// ========================================================================
// ||                             SETUP                                 ||
// ========================================================================
void setup() {
  char s_month[5];
  int tmonth, tday, tyear, thour, tminute, tsecond;
  static const char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";

  // init
  initWiFi();
  initMQTT();

  // init the serial port to be used as a display return
  Wire.begin();
  //Wireling.begin();

  // while (!SerialMonitorInterface && millis() < 5000); //This will block until the Serial Monitor is opened on TinyScreen+/TinyZero platform!

  // Wireling.selectPort(pulseSensorPort);
  // if (pulseSensor.begin()) {
  //   while (true) {
  //     SerialMonitorInterface.println("MAX30101 Wireling not detected!");
  //     delay(1000);
  //   }
  // }

  SerialMonitorInterface.begin(20000);
  while (!SerialMonitorInterface) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  /* Check if SD card is working */
  //SerialMonitorInterface.println("Initializing SD card...");

  // if (!SD.begin(10)) {
  //   SerialMonitorInterface.println("Failed");
  //   while(true);
  // }

  /* RTC */
  rtc.begin();
  // Set the time and date. Change this to your current date and time.
  // setTime(16,19,00,12,3,2016);    //values in the order hr,min,sec,day,month,year

  // __DATE__ is a C++ preprocessor string with the current date in it.
  // It will look something like 'Mar  13  2016'.
  // So we need to pull those values out and convert the month string to a number.
  sscanf(__DATE__, "%s %d %d", s_month, &tday, &tyear);

  // Similarly, __TIME__ will look something like '09:34:17' so get those numbers.
  sscanf(__TIME__, "%d:%d:%d", &thour, &tminute, &tsecond);

  // Find the position of this month's string inside month_names, do a little
  // pointer subtraction arithmetic to get the offset, and divide the
  // result by 3 since the month names are 3 chars long.
  tmonth = (strstr(month_names, s_month) - month_names) / 3;

  months = tmonth + 1;  // The RTC library expects months to be 1 - 12.
  days = tday;
  years = tyear - 2000; // The RTC library expects years to be from 2000.
  hours = thour;
  minutes = tminute;
  seconds = tsecond;

  rtc.setTime(hours, minutes, seconds);
  rtc.setDate(days, months, years);

  /* Display */
  display.begin();
  display.setFont(thinPixel7_10ptFontInfo);
  display.setBrightness(10);
  display.setFlip(true);

  SerialMonitorInterface.println("The Watch is Ready to use");
} 

// ========================================================================
// ||                             MAIN LOOP                              ||
// ========================================================================

  // Check and compare rtc time to set alarm at a specific interval
  /**/

void loop() {

 
  // Put both of this in every loop and page section to can check status with MQTT IMPORTANT
  client.loop();
  if (!client.connected()) {
    SerialMonitorInterface.println("I have disconnected");
    reconnectMQTT();
  }

  // client.publish("Hello/Andrino", "Yes");


  if (currPage == LOCK_SCREEN) {
    // start tracking the milliseconds
    page_LockScreen();
  } else {
    switch (currPage) {
      // Main Screens
      case LOCK_SCREEN: page_LockScreen(); break;
      case ROOT_MENU: page_RootMenu(); break;
      case SUB_MENU1: page_SubMenu1(); break;
      case SUB_MENU2: page_SubMenu2(); break;
      case SUB_MENU3: page_SubMenu3(); break;

      // Education Screens
      case HOOTHOOT_START_SCREEN: page_HootHootStart(); break;
      case HOOTHOOT_QUIZ_SCREEN: page_HootHootQuiz(); break;
      case HOOTHOOT_SUBMISSION_SCREEN: page_HootHootSubmission(); break;
      case SILENTHELPER_SCREEN: page_SilentHelper(); break;
      case ATTENDANCE_SCREEN: page_Attendance(); break;
      case TELEBOT_SCREEN: page_Telebot(); break;
      case GOOGLE_SCREEN: page_GoogleCalendar(); break;
      case SMS_SCREEN: page_SMS(); break;

      // System Screens
      case WEATHER_SCREEN: page_Weather(); break;
      case ALARM_SCREEN: page_Alarm(); break;
      case OXIMETER_SCREEN: page_Oximeter(); break;

      // Entertainment Screens
      case AUDIO_SCREEN: page_Audio(); break;
      case GAME_SCREEN: page_Game(); break;
    }
  }

}

// =========================================================================
// ||                          PAGE - LOCK SCREEN                          ||
// =========================================================================
void page_LockScreen(void) {
  static bool isOn = true;
  // flag for updating the display
  boolean updateDisplay = true;
  boolean updateDynamicSection = true;

  // tracks when entered top of loop
  uint32_t loopStartMs;

  //tracks button states
  boolean btn_Accept_WasDown = false;
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
      //printBtnArrows();
    }

    months = rtc.getMonth();
    days = rtc.getDay();
    years = rtc.getYear();
    hours = rtc.getHours();
    minutes = rtc.getMinutes();
    seconds = rtc.getSeconds();

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
      clock_brightness = hours + 3; // 0 hours = 3 brightness, noon = 15
    else if (hours >= 18)
      clock_brightness = (24 - hours) * 2 + 2;  // 23 hours = 4 brightness, 18 hours = 14
    else
      clock_brightness = 15; // full brightness all afternoon
    if (clock_brightness < 3)
      clock_brightness = 3;
    if (clock_brightness > 15)
      clock_brightness = 15;
    display.setBrightness(clock_brightness);
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

    if (hours == alarmHour && minutes == alarmMinute) { //

        if (isOn) {
          //display.setBrightness(0);
          //updateDynamicSection = true;
          display.off(); // Turn off the display
        } else {
          //display.setBrightness(20);
          display.setCursor(32,45);//display.setCursor(32, 45);
          display.print("Wake up!!");
          //updateDynamicSection = true;
          display.on(); // Turn off the display
        }

        isOn = !isOn;
      }
  }

    
    /*
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
    */


    // capture button down states
    if (btnIsDown(BTN_ACCEPT)) { btn_Accept_WasDown = true; }
    if (btnIsDown(BTN_CANCEL)) { btn_Cancel_WasDown = true; }

    // move to the root menu
    if (btn_Cancel_WasDown && btnIsUp(BTN_CANCEL)) {
      display.setBrightness(15);
      btn_Cancel_WasDown = false;
    }

    // move to the root menu
    if (btn_Accept_WasDown && btnIsUp(BTN_ACCEPT)) {
      currPage = ROOT_MENU;
      return;
    }
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
  boolean updateDynamicSection = true;

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

      // clear the display
      clearScreen();

      // start the display
      display.begin();
      display.setBrightness(10);  // Set brightness level (0-100)

      // print arrow buttons
      printBtnArrows();

      // menu title
      display.setCursor(0, 0);
      display.print("[ MAIN MENU ]");

      // print a divider line
      printDivider();

      // print a divider line
      display.setCursor(0, 50);
      printDivider();
    }

    if (updateDynamicSection) {
      // print the items
      display.setCursor(0, 10);
      printSelected(1, root_Pos);
      display.print("Education");

      display.setCursor(0, 20);
      printSelected(2, root_Pos);
      display.print("System");

      display.setCursor(0, 30);
      printSelected(3, root_Pos);
      display.print("Entertainment");
    }

    // capture button down states
    if (btnIsDown(BTN_UP)) { btn_Up_WasDown = true; }
    if (btnIsDown(BTN_DOWN)) { btn_Down_WasDown = true; }
    if (btnIsDown(BTN_ACCEPT)) { btn_Accept_WasDown = true; }


    // move the pointer down
    if (btn_Down_WasDown && btnIsUp(BTN_DOWN)) {
      if (root_Pos == ROOT_MENU_COUNT) {
        root_Pos = 1;
      } else {
        root_Pos++;
      }
      updateDynamicSection = true;
      btn_Down_WasDown = false;
    }

    // move the pointer up
    if (btn_Up_WasDown && btnIsUp(BTN_UP)) {
      if (root_Pos == 1) {
        root_Pos = ROOT_MENU_COUNT;
      } else {
        root_Pos--;
      }
      updateDynamicSection = true;
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
  while (!SerialMonitorInterface)
    ;  // Wait until serial is ready

  SerialMonitorInterface.println("Starting WiFi initialization...");

  WiFi.setPins(8, 2, A3, -1);  // VERY IMPORTANT FOR TINYDUINO

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

      client.subscribe("HootHoot/Start");
      SerialMonitorInterface.println("HootHoot/Start");

      client.subscribe("HootHoot/Question1/Option1");
      SerialMonitorInterface.println("HootHoot/Question1/Option1");

      client.subscribe("HootHoot/Question1/Option2");
      SerialMonitorInterface.println("HootHoot/Question1/Option2");

      client.subscribe("HootHoot/Question1/ProfAnswer");
      SerialMonitorInterface.println("HootHoot/ProfAnswer");
      
      client.subscribe("tele/Announcement");
      SerialMonitorInterface.println("tele/Announcement");

      // Announcement & SuggestedResponse
      client.subscribe("tele/jake");
      SerialMonitorInterface.println("tele/jake");


      client.subscribe("Weather/Response");
      SerialMonitorInterface.println("Weather/Response");

      client.subscribe("tele/SuggestedResponse");
      SerialMonitorInterface.println("tele/SuggestedResponse");

      break;
    }

    SerialMonitorInterface.println(client.state());
    SerialMonitorInterface.println("Failed to connect to MQTT broker. Retrying in 5 seconds...");
    delay(5000);
  }


  // client.subscribe()
  // client.publish()

  // HootHoot Quiz
  
  

}

void reconnectMQTT() {
    // Loop until we're reconnected
    while (!client.connected()) {
        SerialMonitorInterface.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect("mqtt-ardunio-derpee")) {
            SerialMonitorInterface.println("connected");
            // Resubscribe
            client.subscribe("HootHoot/Start");

            client.subscribe("HootHoot/Question1/Option1");

            client.subscribe("HootHoot/Question1/Option2");

            client.subscribe("HootHoot/Question1/ProfAnswer");
            
            client.subscribe("tele/Announcement");

            client.subscribe("tele/jake");

            client.subscribe("Weather/Response");

            client.subscribe("tele/SuggestedResponse");
            // ... add other subscriptions as needed ...
        } else {
            SerialMonitorInterface.print("failed, rc=");
            SerialMonitorInterface.print(client.state());
            SerialMonitorInterface.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
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

  String topicStr = String(topic);
  String payloadStr = String(receivedPayload);



  //getPulseAndBuzzer();
  // payload 
  // if (payloadStr == "True"){
  //   getPulseAndBuzzer();
  // }

  dictionary.add(topicStr, payloadStr);
  dictionary.printAll();

  // Print the received payload
  SerialMonitorInterface.println(receivedPayload);
}

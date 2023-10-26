//-------------------------------------------------------------------------------
//  TinyCircuits ST BLE TinyShield UART Example Sketch
//  Last Updated 2 March 2016
//
//  This demo sets up the BlueNRG-MS chipset of the ST BLE module for compatiblity 
//  with Nordic's virtual UART connection, and can pass data between the Arduino
//  serial monitor and Nordic nRF UART V2.0 app or another compatible BLE
//  terminal. This example is written specifically to be fairly code compatible
//  with the Nordic NRF8001 example, with a replacement UART.ino file with
//  'aci_loop' and 'BLEsetup' functions to allow easy replacement. 
//
//  Written by Ben Rose, TinyCircuits http://tinycircuits.com
//
//-------------------------------------------------------------------------------


#include <SPI.h>
#include <STBLE.h>

#include <Wire.h>
#include <SPI.h>
#include <TinyScreen.h>

//Library must be passed the board type
//TinyScreenDefault for TinyScreen shields
//TinyScreenAlternate for alternate address TinyScreen shields
//TinyScreenPlus for TinyScreen+

TinyScreen display = TinyScreen(TinyScreenDefault);


//Debug output adds extra flash and memory requirements!
// #ifndef BLE_DEBUG
// #define BLE_DEBUG true
// #endif

#if defined (ARDUINO_ARCH_AVR)
#define SerialMonitorInterface Serial
#elif defined(ARDUINO_ARCH_SAMD)
#define SerialMonitorInterface SerialUSB
#endif


uint8_t ble_rx_buffer[21];
uint8_t ble_rx_buffer_len = 0;
uint8_t ble_connection_state = false;
#define PIPE_UART_OVER_BTLE_UART_TX_TX 0

void setup() {
  SerialMonitorInterface.begin(9600);
  //while (!SerialMonitorInterface); //This line will block until a serial monitor is opened with TinyScreen+! //but it will still work if i dont have it turn on
  BLEsetup();

  //set up screen
  Wire.begin();
  display.begin();
  display.setBrightness(15);
  display.setFont(thinPixel7_10ptFontInfo);
  int width=display.getPrintWidth("Begin");
  //setCursor(x,y);//set text cursor position to (x,y)- in this example, the example string is centered
  display.setCursor(48-(width/2),10);
  SerialMonitorInterface.print(width);
  display.print("Begin");
}


void loop() {
  aci_loop();//Process any ACI commands or events from the NRF8001- main BLE handler, must run often. Keep main loop short.
  
  if (ble_rx_buffer_len) {//Check if data is available
    display.clearScreen();
    SerialMonitorInterface.print(ble_rx_buffer_len);
    SerialMonitorInterface.print(" : ");
    SerialMonitorInterface.println((char*)ble_rx_buffer);

    //display on tinyscreen
    display.setCursor(0,0);
    display.print((char*)ble_rx_buffer);
    ble_rx_buffer_len = 0;//clear afer reading
  }

  if (SerialMonitorInterface.available() || display.getButtons(TSButtonLowerLeft) || display.getButtons(TSButtonLowerRight) ) { //Check if serial input is available to send or a button is pressed
    delay(10);//should catch input
    uint8_t sendBuffer[21];
    uint8_t sendLength = 0;
    display.clearScreen();

    while (SerialMonitorInterface.available() && sendLength < 19) {
      sendBuffer[sendLength] = SerialMonitorInterface.read();

      //display on tinyscreen
      //set cursoe to the middle of the screen base on the length of the string
      //int width=display.getPrintWidth((char*)sendBuffer);
      //print the width of the string
      // SerialMonitorInterface.print("width of text");
      // SerialMonitorInterface.println(width);

      //display.setCursor(48-(width/2),10);
      sendLength++;
    }

    if (display.getButtons(TSButtonLowerLeft)) {
      sendBuffer[0] = 'Y';
      sendBuffer[1] = 'E';
      sendBuffer[2] = 'S';
      sendLength = 3; 
    }
    if (display.getButtons(TSButtonLowerRight)) {
      sendBuffer[0] = 'N';
      sendBuffer[1] = 'O';
      sendLength = 2;
    }

    
    if (SerialMonitorInterface.available()) {
      SerialMonitorInterface.print(F("Input truncated, dropped: "));
      if (SerialMonitorInterface.available()) {
        SerialMonitorInterface.write(SerialMonitorInterface.read());
      }
    }

    sendBuffer[sendLength] = '\0'; //Terminate string
    sendLength++;

    //int width=display.getPrintWidth((char*)sendBuffer);
    //display.setCursor(48-(width/2),10);
    display.setCursor(0,10);
    display.print((char*)sendBuffer);
    
    if (!lib_aci_send_data(PIPE_UART_OVER_BTLE_UART_TX_TX, (uint8_t*)sendBuffer, sendLength))
    {
      SerialMonitorInterface.println(F("TX dropped!"));
    }
  }
  
}


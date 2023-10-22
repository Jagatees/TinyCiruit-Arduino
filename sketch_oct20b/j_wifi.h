
// Network driver for ATMEL WINC1500 module (used on Arduino/Genuino Wifi Shield 101 and MKR1000 boards)
// This library implements a network driver for devices based on the ATMEL WINC1500 wifi module
#include <WiFi101.h>
// A simple and efficient JSON library for embedded C++. 
#include <ArduinoJson.h>


#if defined(ARDUINO_ARCH_SAMD)
  #define SerialMonitorInterface SerialUSB
#else
  #define SerialMonitorInterface Serial
#endif


void initWIFI();
void logicWIFI();
void requestAPI();

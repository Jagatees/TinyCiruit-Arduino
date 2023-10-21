
#include <WiFi101.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>


#if defined(ARDUINO_ARCH_SAMD)
  #define SerialMonitorInterface SerialUSB
#else
  #define SerialMonitorInterface Serial
#endif


void initWIFI();
void logicWIFI();

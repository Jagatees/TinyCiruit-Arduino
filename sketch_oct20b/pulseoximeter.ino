/*************************************************************************
  MAX30101 Wireling Basic Example

  This example prints sensor temperature, BPM, saturated oxygen level, and
  cardiogram output to the Serial Monitor.

  BPM and pulse oxygen output is not for medical use!

  Original MAX30100 code by Raivis Strogonovs:
  https://morf.lv/implementing-pulse-oximeter-using-max30100

  Updated: 4/19/2019 by Ben Rose for TinyCircuits
 *************************************************************************/

#include <Wire.h>
#include <Wireling.h>

#if defined (ARDUINO_ARCH_AVR)
#define SerialMonitorInterface Serial
#elif defined(ARDUINO_ARCH_SAMD)
#define SerialMonitorInterface SerialUSB
#endif

#include <MAX30101.h>

MAX30101 pulseSensor = MAX30101();
int pulseSensorPort = 0;

void setup() {
  SerialMonitorInterface.begin(9600);
  Wire.begin();
  Wireling.begin();
  delay(200);

  while (!SerialMonitorInterface && millis() < 5000); //This will block until the Serial Monitor is opened on TinyScreen+/TinyZero platform!

  Wireling.selectPort(pulseSensorPort);
  if (pulseSensor.begin()) {
    while (true) {
      SerialMonitorInterface.println("MAX30101 Wireling not detected!");
      delay(1000);
    }
  }
}

void loop() {
  Wireling.selectPort(pulseSensorPort);
  if (pulseSensor.update()) {
    if (pulseSensor.pulseValid()) {
      SerialMonitorInterface.print(pulseSensor.temperature());
      SerialMonitorInterface.print("\t");
      SerialMonitorInterface.print(pulseSensor.BPM());
      SerialMonitorInterface.print("\t");
      SerialMonitorInterface.print(pulseSensor.oxygen());
      SerialMonitorInterface.print("\t");
      SerialMonitorInterface.println(pulseSensor.cardiogram() * 10.0);
    } else {
      SerialMonitorInterface.print(pulseSensor.temperature());
      SerialMonitorInterface.print("\t");
      SerialMonitorInterface.println("-\t-\t-");
    }
  }
  delay(20);//Polling the sensor too often can cause extra noise. The sensor can buffer about 300ms of data with the default settings.
}


#include "wifi.h"

void initWIFI() {
  char ssid[] = "SINGTEL-C8NA";  //  your network SSID (name)
  char wifiPassword[] = "57hhcumfd8";  // your network password

  SerialMonitorInterface.begin(9600);
  WiFi.setPins(8, 2, A3, -1); // VERY IMPORTANT FOR TINYDUINO
  while(!SerialMonitorInterface);

  // Attempt to connect to Wifi network:
  SerialMonitorInterface.print("Connecting Wifi: ");
  SerialMonitorInterface.println(ssid);

  // Connect to WiFi, and loop until connection is secured
  WiFi.begin(ssid, wifiPassword);
  while (WiFi.status() != WL_CONNECTED){
    SerialMonitorInterface.print(".");
    delay(500);
  }

  // Print out the local IP address
  SerialMonitorInterface.println("");
  SerialMonitorInterface.println("WiFi connected");
  SerialMonitorInterface.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  SerialMonitorInterface.println(ip);
}



void logicWIFI() {
  SerialMonitorInterface.print("Main loop entered. Now that we're connected, let's do something cool.");
  delay(60000); // Wait a minute before going back through the main loop
}
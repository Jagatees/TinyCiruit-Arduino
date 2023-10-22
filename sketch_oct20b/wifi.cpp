
#include "wifi.h"
// jsonplaceholder.typicode.com

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
  SerialMonitorInterface.print("Main loop entered. Now that we're connected, let's do something cool.\n");

  // Make an HTTP GET request to the JSONPlaceholder API to fetch posts
  WiFiClient client;
  if (client.connect("worldtimeapi.org", 80)) {
    SerialMonitorInterface.println("Connected to JSONPlaceholder API");

    String URI = "/api/timezone/asia/kolkata";
    const char * Hostname = "worldtimeapi.org";


    // Send HTTP request
    client.println("GET " + URI + " HTTP/1.0");
    client.println("Host: " + (String)Hostname);
    client.println("Connection: close");
    client.println();

    // Check HTTP status
    char status[32] = {0};
    client.readBytesUntil('\r', status, sizeof(status));
    if (strcmp(status, "HTTP/1.0 200 OK") != 0) {
      Serial.print(F("Unexpected response: "));
      Serial.println(status);
      client.stop();
      return;
    }

    // Skip HTTP headers
    char endOfHeaders[] = "\r\n\r\n";
    if (!client.find(endOfHeaders)) {
      Serial.println(F("Invalid response"));
      client.stop();
      return;
    }

    

    // Read and print the response
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        SerialMonitorInterface.print(c); // Print the response content

        
      }
    }
    client.stop();
    SerialMonitorInterface.println("\nAPI request done");
  } else {
    SerialMonitorInterface.println("Failed to connect to JSONPlaceholder API");
  }

  delay(60000); // Wait a minute before going back through the main loop
}
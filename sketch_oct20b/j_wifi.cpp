#include "j_wifi.h"
// jsonplaceholder.typicode.com

void j_initWIFI() {
  // Moden + Router
  char ssid[] = "SINGTEL-C8NA";  //  your network SSID (name)
  char wifiPassword[] = "57hhcumfd8";  // your network password

  // char ssid[] = "Jagatees-Phone";  //  your network SSID (name)
  // char wifiPassword[] = "1234567890";  // your network password

  SerialMonitorInterface.begin(9600);
  WiFi.setPins(8, 2, A3, -1); // VERY IMPORTANT FOR TINYDUINO
  while(!SerialMonitorInterface);

   // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

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


  // Excellent Signal: -30 dBm to -50 dBm
  // Good Signal: -51 dBm to -70 dBm
  // Fair Signal: -71 dBm to -90 dBm
  // Poor Signal: -91 dBm to -100 dBm
  // No Signal: -100 dBm and below
  long rssi = WiFi.RSSI();
  SerialMonitorInterface.print("signal strength (RSSI):");
  SerialMonitorInterface.println(rssi);

  // 1 = WEP (Wired Equivalent Privacy)
  // 2 = WPA (WiFi Protected Access) [Jagatees House Wifi]
  // 3 = WPA3 (WiFi Protected Access 3)
  // 4 = Open Network (No Encryption)
  byte encryption = WiFi.encryptionType();
  SerialMonitorInterface.print("Encryption Type:");
  SerialMonitorInterface.println(encryption, HEX);
  SerialMonitorInterface.println();

}

void j_makeGetRequest() {

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


    // https://arduinojson.org/v6/assistant/#/step1
    // SAMD21 
    StaticJsonDocument<768> doc;
    DeserializationError error = deserializeJson(doc, client);

    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }

    const char* abbreviation = doc["abbreviation"]; // "IST"
    SerialMonitorInterface.println(abbreviation);
    client.stop();

  } else {
    SerialMonitorInterface.println("Connection failed.");
  }

  client.stop();
}


void j_logicWIFI() {
  SerialMonitorInterface.print("Main loop entered. Now that we're connected, let's do something cool.\n");

  // j_makeGetRequest();

  delay(60000); // Wait a minute before going back through the main loop
}





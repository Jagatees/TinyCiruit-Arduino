#include <WiFi101.h>
#include <PubSubClient.h>



// Talk and get value from mqtt

// Send value to MQTT
// client.publish("Request/Time", "True");

// Subscribe to MQTT 
// client.subscribe("Request/Result");

//WiFi details
const char* ssid = "Jagatees-Phone";
const char* wifiPassword = "1234567890";


// Create an instance of WiFiClient
WiFiClient espClient;

// MQTT broker details
const char* mqttServer = "172.20.10.2";
const int mqttPort = 1883;

// Create an instance of PubSubClient
PubSubClient client;

#if defined(ARDUINO_ARCH_SAMD)
  #define SerialMonitorInterface SerialUSB
#else
  #define SerialMonitorInterface Serial
#endif

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

void setup() {
    initWiFi();

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
    client.subscribe("Request/Weather/Result"); 
    // client.subscribe("Request/Result2");  // QoS level 1

    SerialMonitorInterface.println("Subscribed to test/debug");
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





void loop() {
    client.loop();

    delay(5000);

    client.publish("Request/Weather", "True");
    // client.publish("test/debug", "Debug message from Arduino");

    //SerialMonitorInterface.println("Published a message to test/topic123");
}

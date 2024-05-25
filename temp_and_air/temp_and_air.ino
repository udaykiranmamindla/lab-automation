#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <DHT11.h>
WiFiClient  client;
#define channel 2357441


// Replace with your network credentials
const char *ssid = "HITAM_S18";
const char *password = "hitam@S18";

// Replace with your ThingSpeak channel settings

const char *thingSpeakApiKey = "4JDEA7YCMEHDFITG";


// Analog pin connected to the A0 pin of MQ-02
const int mq02Pin = A0;
#include <DHT11.h>

// Create an instance of the DHT11 class.
// - For Arduino: Connect the sensor to Digital I/O Pin 2.
// - For ESP32: Connect the sensor to pin GPIO2 or P2.
// - For ESP8266: Connect the sensor to GPIO2 or D4.
DHT11 dht11(2);

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Initialize ThingSpeak
  ThingSpeak.begin(client);
}

void loop() {
  int mq02Value = analogRead(mq02Pin);

  // Convert analog value to parts per million (PPM) using your sensor's calibration data
  float ppm = map(mq02Value, 0, 1023, 0, 5000);  // Adjust the mapping based on your sensor's characteristics

  // Print PPM value to Serial Monitor
  Serial.print("MQ-02 PPM: ");
  Serial.println(ppm);
  float temperature = dht11.readTemperature();

    // Check the result of the reading.
    // If there's no error, print the temperature value.
    // If there's an error, print the appropriate error message.
    if (temperature != DHT11::ERROR_CHECKSUM && temperature != DHT11::ERROR_TIMEOUT)
    {
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.println(" °C");
    }
    
        Serial.println(DHT11::getErrorString(temperature));
  // Update ThingSpeak channel with PPM value
  ThingSpeak.setField(1, ppm);
  ThingSpeak.setField(2, temperature);

  int status = ThingSpeak.writeFields(channel, thingSpeakApiKey);

  if (status == 200) {
    Serial.println("Channel update successful");
  } else {
    Serial.println("Problem updating channel. HTTP error code " + String(status));
  }
  delay(15000);
}  

 

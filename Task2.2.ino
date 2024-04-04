#include <Wire.h>
#include <BH1750.h>
#include <WiFiNINA.h>
#include <ThingSpeak.h>
#include "secrets.h" // Make sure you have created this file with your WiFi and ThingSpeak credentials

// Initialize the BH1750 sensor
BH1750 lightMeter;

// Initialize the WiFi client library
char ssid[] = SECRET_SSID;       // your network SSID (name)
char pass[] = SECRET_PASS;       // your network password
int keyIndex = 0;                // your network key Index number (needed only for WEP)
unsigned long myChannelNumber = SECRET_CHANNEL_NUMBER; // your channel number
const char * myWriteAPIKey = SECRET_WRITE_API_KEY; // your channel write API key

WiFiClient  client;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // Attempt to connect to WiFi network
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, pass);
    Serial.print(".");
    delay(5000);
  }
  Serial.println("\nConnected to WiFi");
  ThingSpeak.begin(client); // Initialize ThingSpeak

  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println(F("BH1750 Test begin"));
  } else {
    Serial.println(F("Error initializing BH1750"));
  }
}

void loop() {
  // Read from the BH1750 sensor
  float lux = lightMeter.readLightLevel();

  // Print out the value
  Serial.print("Light Level: ");
  Serial.print(lux);
  Serial.println(" lx");

  // Check if the sensor reading is valid (i.e., the read value is not an error code).
  if(lux >= 0) {
    // Send data to ThingSpeak
    ThingSpeak.setField(1, lux); // Field 1 for light level in your ThingSpeak channel
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  } else {
    Serial.println(F("[BH1750] ERROR: cannot read data"));
  }

  // Wait 60 seconds before sending the data again
  delay(60000);
}

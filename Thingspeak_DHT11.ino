#include <WiFi.h>
#include "ThingSpeak.h"
#include "DHT.h"

#define DHTPIN 14

char ssid[] = "MAKERINDO2";      // your network SSID (name)
char pass[] = "makerindo2019";   // your network password
int keyIndex = 0;                // your network key Index number (needed only for WEP)
WiFiClient  client;
DHT dht(DHTPIN, DHT11);

unsigned long myChannelNumber = 1837462;           // replace 0000000 with your channel number
const char * myWriteAPIKey = "ZH4S5QOIE1TFQWS3";   // replace XYZ with your channel write API Key


void setup() {
  Serial.begin(9600);        //Initialize serial
  dht.begin();
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F"));
  
  // set the fields with the values
  ThingSpeak.setField(1, t);
  ThingSpeak.setField(2, h);

  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200) {
    Serial.println("Channel update successful.");
  }
  else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  delay(15000); // Wait 10 seconds to update the channel again
}

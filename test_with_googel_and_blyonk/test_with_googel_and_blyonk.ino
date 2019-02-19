#include <ESP8266WiFi.h>
#include <String.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
/////////////////////////////////////////
#define WLAN_SSID       "Aditya"
#define WLAN_PASS       "zmanaditya"
////////////////////////////////////////
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   
/////////////////////////////////////////////
#define IO_USERNAME  "Aditya_rasp_test"
#define IO_KEY       "71c0520d6f314be692b6b309c0acd392"
/////////////////////////////////////////
#define pin1            D4
#define pin2            D0
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, IO_USERNAME, IO_KEY);

////////////////////////////////////
Adafruit_MQTT_Subscribe Switch = Adafruit_MQTT_Subscribe(&mqtt, IO_USERNAME "/feeds/test");
Adafruit_MQTT_Subscribe Switch2 = Adafruit_MQTT_Subscribe(&mqtt, IO_USERNAME "/feeds/test1");
void MQTT_connect();
void setup()
{

  Serial.begin(9600);
  delay(10);
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
  mqtt.subscribe(&Switch);
  mqtt.subscribe(&Switch2);
}

void loop()
{
  MQTT_connect();
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    // Check if its the onoff button feed
if (subscription == &Switch) {
      Serial.print(F("On-Off button on pin1: "));
      Serial.println((char *)Switch.lastread);

      if (strcmp((char *)Switch.lastread, "1") == 0) {
        digitalWrite(pin1, HIGH);
      }
      if (strcmp((char *)Switch.lastread, "0") == 0) {
        digitalWrite(pin1, LOW);
      }
}
if (subscription == &Switch2) {
      Serial.print(F("On-Off button on pin2: "));
      Serial.println((char *)Switch2.lastread);

      if (strcmp((char *)Switch2.lastread, "0") == 0) {
        digitalWrite(pin2, HIGH);
      }
      if (strcmp((char *)Switch2.lastread, "1") == 0) {
        digitalWrite(pin2, LOW);
      }
}
  }

}



void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
}

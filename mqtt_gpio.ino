#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

#include <EspMQTTClient.h>
#include <ArduinoJson.h>
#include <Vector.h>
#include "settings.h" // This file is meant to contain settings like WIFI settings

EspMQTTClient client(
  WIFI_SSID,
  WIFI_PASSWORD,
  MQTT_BROKER_HOST,
  MQTT_BROKER_USERNAME,
  MQTT_BROKER_PASSWORD,
  DEVICE_NAME,
  MQTT_BROKER_PORT
);

void setup() {
  Serial.begin(115200);
  Serial.print("Connecting as ");
  Serial.println(DEVICE_NAME);

# ifdef DEBUG
  client.enableDebuggingMessages();
# endif
}

void handleGpio(const String &payload) {
  DynamicJsonDocument doc(512);
  deserializeJson(doc, payload);

  uint8_t pin = doc["pin"];
  uint8_t value = doc["value"];

  pinMode(pin, OUTPUT);
  digitalWrite(pin, value);
}

#ifndef ESP32 // As the ESP32 has not implemented the analogWrite yet
void handlePwm(const String &payload) {
  DynamicJsonDocument doc(512);
  deserializeJson(doc, payload);


  uint8_t pin = doc["pin"];
  uint16_t value = doc["value"];

  pinMode(pin, OUTPUT);
  analogWrite(pin, value);
}
#endif

void onConnectionEstablished() {
  Serial.println("Ready");

  client.publish(DEVICE_NAME "/application", "mqtt_gpio", true);
  client.subscribe(DEVICE_NAME "/gpio", handleGpio);
# ifndef ESP32 // As the ESP32 has not implemented the analogWrite yet
  client.subscribe(DEVICE_NAME "/pwm", handlePwm);
# endif
}

void loop() {
  client.loop();
}

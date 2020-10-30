#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

#include <EspMQTTClient.h>
#include <ArduinoJson.h>
#include <Vector.h>
#include "settings.h" // This file is meant to contain settings like WIFI settings

enum PinMode
{
  UNSET = 0,
  DIGITAL_OUT = 1,
  PWM_OUT = 2,
  DIGITAL_IN = 3
};

PinMode modes[100];
int states[100];

EspMQTTClient client(
    WIFI_SSID,
    WIFI_PASSWORD,
    MQTT_BROKER_HOST,
    MQTT_BROKER_USERNAME,
    MQTT_BROKER_PASSWORD,
    DEVICE_NAME,
    MQTT_BROKER_PORT);

void setup()
{
  Serial.begin(115200);
  Serial.print("Connecting as ");
  Serial.println(DEVICE_NAME);

#ifdef DEBUG
  client.enableDebuggingMessages();
#endif
}

void handleGpio(const String &topicStr, const String &payload)
{
  String pinStr = String(topicStr);

  pinStr.replace(DEVICE_NAME "/", "");
  pinStr.replace("/state", "");
  int pin = pinStr.toInt();
  int state = payload.toInt();

  if (pin > 99 || modes[pin] == PinMode::DIGITAL_IN)
  {
    return;
  }

  states[pin] = state;
}

void handleSetMode(const String &topicStr, const String &payload)
{
  String pinStr = String(topicStr);

  pinStr.replace(DEVICE_NAME "/", "");
  pinStr.replace("/state", "");
  int pin = pinStr.toInt();

  if (pin > 99)
  {
    return;
  }

  modes[pin] = translateStringToEnum(payload);
}

PinMode translateStringToEnum(const String &name)
{
  if (name == "DIGITAL_OUT")
    return PinMode::DIGITAL_OUT;

  if (name == "PWM_OUT")
    return PinMode::PWM_OUT;

  if (name == "DIGITAL_IN")
    return PinMode::DIGITAL_IN;

  return PinMode::UNSET;
}

void onConnectionEstablished()
{
  Serial.println("Ready");

  client.publish(DEVICE_NAME "/__application", "mqtt_gpio", true);
  client.subscribe(DEVICE_NAME "/+/state", handleGpio);
  client.subscribe(DEVICE_NAME "/+/mode", handleSetMode);
}

void handlePins()
{
  for (int i = 0; i < 100; i++)
  {
    if (modes[i] == DIGITAL_OUT)
    {
      handleDigitalOutput(i);

      continue;
    }

    if (modes[i] == PWM_OUT)
    {
      handlePwmOutput(i);

      continue;
    }

    if (modes[i] == DIGITAL_IN)
    {
      handleDigitalInput(i);

      continue;
    }
  }
}

void handleDigitalOutput(int pin)
{
  pinMode(pin, OUTPUT);
  digitalWrite(pin, states[pin] > 0 ? 1 : 0);
}

void handlePwmOutput(int pin)
{
  pinMode(pin, OUTPUT);

#ifndef ESP32
  analogWrite(pin, states[pin]);
#else
  digitalWrite(pin, 0);
#endif
}

void handleDigitalInput(int pin)
{
  pinMode(pin, INPUT);
  int lastState = states[pin] > 0 ? 1 : 0;
  states[pin] = digitalRead(pin);

  if (lastState == states[pin])
  {
    return;
  }

  String publishBase = String(DEVICE_NAME "/");

  client.publish(publishBase + pin + "/toggle", "", false);
  client.publish(publishBase + pin + "/state", String(states[pin]), true);

  if (states[pin] == 0)
  {
    client.publish(publishBase + pin + "/toggle/off", "", false);

    return;
  }

  client.publish(publishBase + pin + "/toggle/on", "", false);
}

void loop()
{
  client.loop();
  handlePins();
}

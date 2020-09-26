# MQTT GPIO

This is a MQTT based controller for NeoPixels with the ESP8266/ESP32 (and propably others as well).

## Prerequisites

- A MQTT broker
- A set-up Arduino IDE for flashing ESP82xx or ESP32 controllers

## Setup

1. Clone the repository
2. Run the `./configure` (do a `chmod +x` before if it failes)
    1. (alternative) copy or create a settings.h file. You can use the settings.h.dist as a template
3. Flash the sketch trough the Arduino IDE

! Note that you have to run those commands in the shell (Mac/Linux) or in the GIT bash or Linux Subsystem (Windows) depening on your platform.

## What it can do

- Set GPIO pin outputs
  - PWM
  - 1/0

## MQTT events

### `DEVICE_NAME "/gpio"`

This action sets a gpio pin to HIGH or LOW.

#### Parameters

The parameter are meant to be sent in a JSON object.

| Name  | Type             | Description            |
| ----- | ---------------- | ---------------------- |
| pin   | Number (0 - 255) | The pin (GPIO) number for the output pin (e.g. D3 is not GPIO number 3, so take a look into the pinout reference of your chip). |
| value | Number (0 / 1)   | The value for the pin. |

### `DEVICE_NAME "/pwm"`

This action sets a gpio pin to HIGH or LOW.

#### Parameters

The parameter are meant to be sent in a JSON object.

| Name  | Type              | Description                |
| ----- | ----------------- | -------------------------- |
| pin   | Number (0 - 255)  | The pin (GPIO) number for the output pin (e.g. D3 is not GPIO number 3, so take a look into the pinout reference of your chip). |
| value | Number (0 - 1023) | The pwm value for the pin. |

#### !! Limitation !!

The ESP32 does not implement the `analogWire` function yet (which is used for the PWM), so the PWM event is not available there.

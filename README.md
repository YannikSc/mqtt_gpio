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
- Emit MQTT events on input (change)

## MQTT events

### `DEVICE_NAME "/PIN_NUMBER/state"`

This action sets the state for `*_OUT` pins (`PWM_OUT` and `DIGITAL_OUT`).

#### Values

- For digital: 1/0
- For PWM: 0 - 1023

### `DEVICE_NAME "/PIN_NUMBER/mode"`

This action sets a gpio pin to HIGH or LOW.

#### Values

- `DIGITAL_OUT`
- `PWM_OUT`
- `DIGITAL_IN`
- (`UNSET` aka anything else) - As all other modes cause the pin to be set/read each iteration (`loop()`) this mode can be used to disable a pin. This will not set the pin to 0 but will stop updating it.

#### !! Limitation !!

The ESP32 does not implement the `analogWire` function yet (which is used for the PWM), so the PWM event is not available there. Also, the PWM mode will set a output to digital 0 if used as otherwise devices may overload/overheat (e.g. an uncooled high-power LED).

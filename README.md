# ESPHome Remeha Thermostat

A fully local thermostat component for Remeha central heaters and heat pumps.

## Requirements

This component depends on the [Remeha Confida](https://github.com/robertklep/esphome-remeha-confida) component that supports local access to Remeha devices over BLE (Bluetooth Low Energy). I'm linking to my fork of the component since it has some additional features implemented that are required for this thermostat component to work. Once the developers of the original have merged my PR's, you can also use the original.

Since most Remeha devices don't support BLE out-of-the-box, you need to buy an additional module to add BLE support. The cheapest (~€150), and certainly easiest, solution for this is the Remeha Home Hub. Although this device is often marketed together with the qSense thermostat, you only need the Home Hub. It connects to an easily accessible service port on your Remeha device and doesn't require any tools or dismantling. All devices that are supported by the Home Hub should also be supported by this component.

Other than this BLE add-on for your Remeha device, the only other requirement is an ESP32. It doesn't really matter which model, as long as it supports WiFi and BLE.

## Setup

Setup consists of two parts:
* Setting up the Confida component so it can communicate with your Remeha device over BLE. See [the documentation](https://github.com/robertklep/esphome-remeha-confida#remeha-confida-uitlezen-met-een-esp32-esphome).
* The thermostat component requires two sensors to be configured (current and target temperature), and one optional sensor (if there's currently a demand for heat or not).

You can use the example YAML files from the Confida component for initial setup, then add the thermostat component and configuration to it once the communication between the ESP32 and the Remeha device are working. NOTE: the example configurations don't configure the "Temporary room setpoint" `number` and "Heat demand" `binary_sensor`. You will need to make sure to add those to the existing configuration. See the included YAML example for their setup.

A minimal YAML example is available in this repository. This example only implements the minimal requirements for the thermostat component, and works with the Home Hub connected to my Elga Ace All-in-One.

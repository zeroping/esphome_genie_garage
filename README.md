# ESPHome Genie Garage Door Opener Interface
ESPHome config and code for a serial interface for some Genie garage door openers

## What is this?

Some Genie garage door openers, like the Model 3024, have a 'Network' port that is intended to be used with a non-wifi wireless adapter. It would normally be used for things like thier two-way handheld remote, which gives the user confirmation that the garage door is closed.

The actual 'Network' port is just a 10-pin 0.1" connector, and provides 3.3 V power and some UART serial data lines. It's designed for a Genie wireless adapter that talks to some Genie remotes. It can be used to retrieve the garage opener state.

[ESPHome](https://esphome.io/index.html) is a system for flashing ESP32- and ESP8266-based IoT home automation devices. In this project, we use an [ESP8266-based module](https://www.waveshare.com/ESP-01F.htm) to listen to the serial data coming out of the garage door opener, and report that over WiFi to wherever we want (such as [HomeAssistant](https://www.home-assistant.io/)).

## The Bad News

Right now, we can't control the garage door. We can only listen to the state.

Additionally, the communication between the garage door powerhead and the [GN-BX network adapter](https://us.amazon.com/Genie-Garage-Openers-Network-Adapter/dp/B005X0IPIG) is authenticated. We can't get data out of the powerhead until it sees a correct response to some packets. As a result, we need a GN-BX to start things off, so we can then listen in to the packets.

## The Good News

Once we get status info flowing out of the garage door opener, we can decode it and do whatever we want with it! This includes:
- Door Open/closed state
- Moving up/down state
- Overhead light state
- Break-beam sensor state

## What's in this repo

[protocol-notes.md](/protocol-notes.md) contains some notes from decoding the serial protocol Genie uses
[electrical-notes.md](/electrical-notes.md) contains some info about the electrical pinouts of the system, and how to solder an ESP01F to a GN-BX network adapter
[garagedoor.yaml](/garagedoor.yaml) is an ESPHome config for flashing an ESP-01F module.
[genie_garage_serial_component.h](/genie_garage_serial_component.h) is the C++ code used to make a custom component which does all the data decoding. Used by garagedoor.yaml.

## Future Work

Right now, this requires buying a GN-BX Network Adapter. If we can figure out how the authentication works between the network adapter and the powerhead, then we can have the ESPHome device directly authenticate.

Even without that, we could decode the commands needed to control the garage door opener. A module like the ESP32 can support dual UARTs, so we could man-in-the-middle the GN-BX's serial line to inject our own commands.

Finally, there's more protocol decoding work that could be done, but we already know almost everything we could want about the status.




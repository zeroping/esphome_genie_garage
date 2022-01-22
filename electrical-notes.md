# Electrical Notes

## List of known supported garage door openers so far:
- Model 3024

## GN-BX details

<img src="/assets/gnbx-internal.png" alt="GN-BX internals" width="50%"/>

This is what the network port is designed for, the Genie Garage Door GN-BX Network Adaptor.
- Model 37350R 
- FCC ID [B8Q-915AD](https://fccid.io/B8Q-915AD) (includes internal photos)

It's a 433MHz wireless adapter that's used for [a remote that lets you know when the door is closed](https://www.amazon.com/Genie-GLRN-R-Close-Confirm-Adapter/dp/B00715DTKA). Maybe also used for a wall-mounted remote too.

You can buy them from time to time on eBay for <$30

The GN-BX contains a cc1110-f32.

It has an unpopulated 0x10 0.1" connector, inside the case:
1. 3V3
2. Reset
3. P0.4 or U1TX, RX into opener
4. ?
5. ?
6. P0.5 or U1RX, TX from opener
7. P0.3 U0TX
8. P0.2 U0RX
9. GND
10. ?

## Network Port

It's a 2x5 0.1" conenctor.

Pinout:
 1. GND
 2. 3.3V
 3. GND
 4. TX out of garage (3.3 V 38,400 baud 8-N-1)
 5. GND
 6. RX out of adapter (3.3 V 38,400 baud 8-N-1)
 7. GND
 8. Appears to go to cc1110 Pin 31, Reset
 9. GND
10. 3.3V

Viewed from below the opener:
- Pin 1 is bottom left, closest to N in the text 'Network'
- Pin 10 is top right

## Adding an ESP8266 or ESP32 to the GN-BX

<img src="/assets/gn-bx-with-esp.jpg" alt="modified board" width="60%"/>

I used the very tiny ESP-01F. You shouldn't. It's annoyingly small. Get some larger ESP-based board.

Start by flashing the ESP with ESPHome and getting it on your wifi. You have to hookmup a serial adapter for this, and is [covered better elsewhere](https://www.pieterbrinkman.com/2022/01/01/2022-update-flash-esphome-on-esp32-esp2866-nodemcu-board/). Flashing uses the UART pins, and we'll be connecting RX to the Opener's TX pin later, so we do this programming before soldering to the GN-BX

Once it's programed, use the **internal** 10-pin connector from the GN-BX to connect the ESP:
- ESP's GND to pin 9
- ESP's 3.3 V to pin 1
- ESP's RX to pin 6

(be aware the pinout of the two 10-pin connectors are different)

You may want to solder the ESP's RF shield to some of the exposed ground-plane on the GN-BX's circuit board, just to mechanically hold it down.





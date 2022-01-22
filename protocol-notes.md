
# Protocol

## Overall Format
example: 
```
01 0C 61 00 00 A9 2C FD 2A 9A 89 05 91`
01 is constant
   0C  = total length - 1
      61 is a type code?
```

last byte (91) is sum of all previous bytes - 1

Codes are somtimes followed by acks that are 0x80 higher

## Adapter to Opener (powerhead) packets

### Adapter 0x61 - I'm here? 

`01 0C 61 00 00 A9 2C FD 2A 9A 89 05 91`

payload is all 00 when initiaizing - perhaps used for auth?

After that, is sent with real payload once, never seen again

### Adapter 0x70

`01 05 70 00 01 76 01 05 70 00 01 76` (It's getting repeated back-to-back)

payload: `00 01 76 `

Used once in startup

### Adapter 0x21

`01 05 21 00 23 49` (once shortly after start) (not always)

`01 05 21 00 22 48` (repeated)

some kind of keep alive?

### Adapter 0x40

`01 04 40 19 5D`

Maybe used to indicate the paring button command? Not happening all the time

## Opener (powehead) to Adapter packets

### Opener 0x60 - 
```
01 08 60 00 45 EC 45 D9 B7 
01 08 60 00 48 E4 48 D4 B0 
01 08 60 00 54 EC 54 C8 C4
```

Seems to initate startup, payload seem to change on reboot / over time.

Probably the start of authenticaion.

Doesn't roll as often as it should though. Like once per reboot per session?

### Opener 0x62

`01 08 62 00 AB 1A D5 0C 10`

Seen once during startup.

Probably the start of authenticaion.

### Opener 0xF0

`01 04 F0 00 F4`

Only ever once, right at the start

Response to 70?

Oddly only acked once

## Opener 0xA1

`01 04 A1 00 A5`

Direct response to 0x21?

### Opener 0xE1 

`01 04 E1 00 E5`

Only seen at startup

direct response to 0x61?

### Opener 0x20 - Opener status
```
01 0A 20 00 02 48 00 EC 00 00 60
(opening)
01 0A 20 00 03 08 00 EC 00 00 21
01 0A 20 00 13 08 00 E9 00 00 2E (opening)
01 0A 20 00 13 08 00 E0 00 00 25
...
01 0A 20 00 13 08 00 00 00 00 45
01 0A 20 00 13 08 00 00 00 00 45 
01 0A 20 00 10 88 00 00 00 00 C2 (open)
...
01 0A 20 00 11 08 00 00 00 00 43 (closing)
01 0A 20 00 11 08 00 04 00 00 47
01 0A 20 00 11 08 00 0E 00 00 51
...
01 0A 20 00 11 08 00 E9 00 00 2C
01 0A 20 00 12 48 00 EC 00 00 70 (closed)

Opener status
            10  = overhead light on
            02 = last closed (!last open)
            01 = moving
               80 ?
               40 ?
               04 = sensor blocked
                     EC clearly amount of closed-ed-ness
```

### Opener 0x22 
```
01 14 22 00 51 12 44 E8 05 86 11 00 00 00 7B EC 00 EC 00 00 B4
01 14 22 00 51 12 44 E8 05 86 11 00 00 00 7B D4 00 EC 00 00 9C (while opening)
01 14 22 00 51 12 44 E8 05 87 11 00 00 00 7B EC 00 EC 00 00 B5
01 14 22 00 51 12 44 E8 05 87 11 00 00 00 7B EC 00 EC 00 00 B5
01 14 22 00 51 12 44 E8 05 87 11 00 00 00 7B EC 00 EC 00 00 B5
after some hours
01 14 22 00 51 12 44 E9 05 89 11 00 00 00 7B EC 00 EC 00 00 B8
```

Longer opener status

EC seems to be a mirror of 0x20's closed-ed-ness value

### opener 0x70

`01 05 70 00 01 76`

Only once

Unusual, since the adapter says this too

Was said around the time of pairing button


## A guess at the startup process

```adapter: 01 0C 61 00 00 00 00 00 00 00 00 00 6D (I'm me, but I don't have a challange response)
adapter: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 (timing BS? Wakeup?)
opener: 01 04 E1 00 E5 (0x61 ack)
opener: 01 08 60 00 54 EC 54 C8 C4 (I'm an opener, here's an auth challange periodicly until we're set up)
adapter: 01 0C 61 00 00 A9 2C FD 2A 9A 89 05 91 (I'm me, here's an auth challange response)
opener: 01 04 E1 00 E5 (0x61 ack, again)
opener: 01 08 62 00 AB 1A D5 0C 10 (???)
adapter: 01 0C 61 00 00 A9 2C FD 2A 9A 89 05 91 (I'm me, here's a challange response again)
opener: 01 04 E1 00 E5 (0x61 ack, again)
opener: 01 08 62 00 AB 1A D5 0C 10 (???, repeat)
adapter: 01 05 70 00 01 76 
adapter: 01 05 70 00 01 76 (same, repeated back to back!)
opener: 01 04 F0 00 F4 (0x70 ack )
opener: 01 0A 20 00 02 48 00 EC 00 00 60 (opener status start flowing)
adapter: 01 05 21 00 23 49 (adapter status, seems to poll for 0x22 or 0x23)
opener: 01 04 A1 00 A5 (0x 21 ack)
opener: 01 0A 20 00 02 48 00 EC 00 00 60 (opener status)
opener: 01 10 23 00 00 00 00 00 00 00 03 11 24 03 06 10 84
adapter: 01 05 21 00 22 48 (adapter status)
opener: 01 04 A1 00 A5 (0x 21 ack)
opener: 01 0A 20 00 02 48 00 EC 00 00 60 (opener status)
opener: 01 14 22 00 51 12 44 E8 05 86 11 00 00 00 7B EC 00 EC 00 00 B4 (long opener status)
```

It looks like the response is different every time! Crap!

Good news: replaying the adapter 0x61 response works... until the opener is rebooted or changes it's challange.

Looking at multiple inits:

```
01 08 60 -- from opener --    01 0C 61 -- from adapter --               01 08 62 -- from opener --
01 08 60 00 54 EC 54 C8 C4    01 0C 61 00 07 41 BC 0A 69 44 96 A5 63    01 08 62 00 BC 5A 5C C0 9C
01 08 60 00 54 EC 54 C8 C4    01 0C 61 00 C4 C7 31 19 5F 0A 9A BE 03    01 08 62 00 C4 90 80 2C 6A
01 08 60 00 54 EC 54 C8 C4    01 0C 61 00 0D C1 82 0A 93 44 BE A5 01    01 08 62 00 8C 5A E8 C1 F9
01 08 60 00 54 EC 54 C8 C4    01 0C 61 00 E8 A5 D3 3F D6 91 2A 36 D3    01 08 62 00 65 7F 6D 17 D2
01 08 60 00 54 EC 54 C8 C4    01 0C 61 00 E5 C1 D4 69 CB C9 1F A6 A9    01 08 62 00 CF 5F 98 A7 D7
01 08 60 00 54 EC 54 C8 C4    01 0C 61 00 88 F4 DF 7A E7 85 AB 73 CC    01 08 62 00 67 06 50 3F 66
01 08 60 00 54 EC 54 C8 C4    01 0C 61 00 69 F2 5A C0 F3 6F 2D 68 D9    01 08 62 00 72 F3 27 F3 E9
01 08 60 00 54 EC 54 C8 C4    01 0C 61 00 4F 52 6F C5 25 7B B5 E8 7F    01 08 62 00 82 F4 D4 CA 7E    
01 08 60 00 57 EC 57 CB CD    01 0C 61 00 57 61 1F D0 E5 EE 95 24 A0    01 08 62 00 C3 5C 53 F5 D1
```

          
















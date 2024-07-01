# GNSS Timestamp

## Idea
This is intended as a timestamping device, triggered by a external pulse, sends the current timestamp over serial in ISO 8601 format with milliseconds.

It connects to the host computer via USB-serial, most arduinos have this built in.<br>
For testing it can also be triggered by sending any character to it over this serial port.

## Hardware
Pinout<br>
On devices with separate HardwareSerial available, like the [Arduino nano every](https://docs.arduino.cc/hardware/nano-every/):
<pre>
0: TX to GNSS module, not used for now.
1: RX from GNSS module, sends NMEA 0183 to mcu.
</pre>

On SoftwareSerial devices:
<pre>
4: TX to GNSS module
5: RX from GNSS module
</pre>

Common for both:
<pre>
2: PPS from GNSS module, rising pulse that happens exactly at the start of the second.
3: Trigger input, a falling edge sends out the timestamp. Use a optocoupler or similar, uses pullup.
</pre>
The trigger input needs to be debounced properly, else it will trigger many timestamps. Make sure the external signal is conditioned to stay within safe limits of the mcu, avoid connecting directly to it.<br>
Recommended to use a opto coupler on the trigger input to isolate that device from the arduino and host computer. This needs a trigger source that can supply it's own current to the LED inside the opto.<br>

The connection to the GNSS should be 3.3V signals for a MCU that runs on 3.3v, there is versions that run on 5V and the io and supply needs to be managed properly to avoid frying something.

## Building

This project is built using the [platformio ide](https://platformio.org/platformio-ide).<br>
By default it builds for a few platforms as specified in [platformio.ini](platformio.ini), uncomment the `default_envs` and set it to the platform you want to use. This simplifies the build and upload.
Use the `PlatformIO: Upload` with the mcu of choice connected to the computer.

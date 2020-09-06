# waterpump_start
A simple Arduino sketch for smooth start custom water pump based on a brushless drive (e.g. HDD).

Warning: you may need to edit your own Servo.h library to change frequency to 60Hz instead of default 50Hz.

Quick description:

D12 - output of standard ESC control pulses (60Hz, 1 to 2 ms)
D2 - button pin (to the GND), changes speed between three speeds (100%, 75%, and 50% by default)
D13 - LED, indicates startup sequence (turned on while smooth start is working) and speed switch (turns on while a button is pressed)

Tested on these boards: Arduino Nano and Leonardo, Freetronics Etherten and Leostick.

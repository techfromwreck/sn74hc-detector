# sn74hc-detector

Arduino UNO R3 sketch that identifies which 74HC series logic gate chip is inserted into a ZIF socket. Supports 74HC00, 74HC02, 74HC04, 74HC08, and 74HC32.

How it works
Each socket pin (except VCC/GND) connects to an Arduino pin through a current-limiting resistor. The sketch cycles through input combinations on all quad-gate positions in parallel and compares the output pattern against known signatures to identify the chip.

Contents
- src/main.cpp — chip detection sketch
- tools/pinout.sh — script to print reference ASCII pinouts for each chip type
- tools/monitor.sh — script to read and log test results over serial

## Dependencies
- platform IO

## Usage
### Upload
```shell
platformio run -t upload
```
### Run
Start the serial monitor
```shell
./tools/monitor.sh
```
Insert/swap the chip and reset the Arduino.

## Links
- [YouTube](https://www.youtube.com/watch?v=WuEuF12oxDA&t=734s)
- [Tech from Wreck's Google Site](https://sites.google.com/view/techfromwreck/home)

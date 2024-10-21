# ESP32 Arduino AC Controller

This is a simple project that enables E131 control of a relay board via an Arduino-compatible ESP32 board.

Based on Wolf Butler's "Frankenpower Mk. 3" build, but modernized & improved.

## Hardware requirements

_Note: affiliate links below._

* An ESP32 board, such as [this ESP-WROOM-32 board](https://amzn.to/3UkwWFr)
* A relay board, such as [this 8-channel SSR board](https://amzn.to/4hd1DX0)
* A power supply for both boards, such as [this Meanwell 5v ps](https://amzn.to/3BQDsxb)
* And various mounting hardware. I'll take you as far as a bench test - the final product is up to you.

## Usage

* Clone this repo or copy/paste the code into a new sketch
* Install the Espressif ESP32 core using the Arduino IDE Board Manager [(instructions here)](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html)
* Install [@forkineye's ESPAsyncE131 package](http://github.com/forkineye/ESPAsyncE131) in Arduino IDE
* Modify this code to suit your own needs:
  * Set the channels you want to use (`int channels[]`)
  * Set the pins you're using, and in the proper order (`int outputs[]`)
  * Set the number of universes you're using -- the default here (1) is probably fine
  * Create an `arduino_secrets.h` file with your wifi username and password (see the code for usage/constants)
* Set a static IP for the ESP32 (router, private lan, however you want to do it)
* Configure XLights to speak E131 to the controller

### Acknowledgements

- @forkineye for the E131 library & example code <http://github.com/forkineye/ESPAsyncE131>
- Wolf Butler for the idea https://lunardenlights.com/2021/02/frankenpower-mk-iii/
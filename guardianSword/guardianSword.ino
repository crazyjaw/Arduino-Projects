#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 90

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 4

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() {
  delay(500);
  LEDS.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS).setCorrection( TypicalPixelString    );
  LEDS.setBrightness(128);
  //  set_max_power_in_volts_and_milliamps(5, 1000);
}

void loop() {
  int bladeStart = 14;
  int bladeEnd = 83;
  setColors(0, bladeStart, 91);
  setColors(bladeStart, bladeEnd, 185);
  setColors( bladeEnd, NUM_LEDS, 91);
}

void setColors(int startPix, int endPix, int hue) {
  //  static uint8_t hue = 185;
  // First slide the led in one direction
  for (int i = startPix; i < endPix; i++) {

    CHSV color = CHSV(hue, 255, 250);
    leds[i] = color;

    FastLED.show();
  }
}


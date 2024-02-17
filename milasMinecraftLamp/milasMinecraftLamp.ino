#include "avr/interrupt.h";
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <math.h>
#include <avr/sleep.h>

#define LED_PIN 0 // neopixel data pin
#define NUM_PIXELS 8
#define NUM_STRIPS 1 // neopixel data pin
#define NUM_COLORS 4
#define SHOW_PERIOD 5000

volatile int lastEncoded = 0;
volatile bool isOff = true;



// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel Strips[NUM_STRIPS] = {
  strip
};


uint32_t diamond = strip.Color(69, 172, 165);
uint32_t redstone = strip.Color(229, 0, 0);
uint32_t emerald = strip.Color(0, 255, 0);
uint32_t lapis = strip.Color(9 , 48 , 255);
uint32_t colors[4] = {diamond, emerald,  redstone, lapis};
uint32_t lastColor = colors[0];
void setup()
{
  setAllStrips(colors[0]);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  // set pins 3 and 4 to input
  // and enable pullup resisters

}

void loop() {
  for(int i = 0; i < NUM_COLORS; i++) {
    crossFade(lastColor, colors[i], 10UL);
    delay(SHOW_PERIOD);
    lastColor = colors[i];
  }
 
}

void setPixels(uint32_t pos) {

  strip.fill(strip.numPixels(), pos );

  strip.show();
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}


void setAllStrips(const uint32_t color) {
  byte red = (color >> 16) & 0xff;
  byte green = (color >> 8) & 0xff;
  byte blue = color & 0xff;
  setAllStrips(red, green, blue);
}

void setAllStrips(byte red, byte green, byte blue) {
  // for each strip
  for (int strip = 0; strip < NUM_STRIPS; strip++) {
    // for each pixel
    for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {
      Strips[strip].setPixelColor(pixel, red, green, blue);
    }
    Strips[strip].show(); // Output to strip
  }
}

void crossFade(const uint32_t startColor, const uint32_t endColor, unsigned long speed) {
  byte startRed = (startColor >> 16) & 0xff;
  byte startGreen = (startColor >> 8) & 0xff;
  byte startBlue = startColor & 0xff;

  byte endRed = (endColor >> 16) & 0xff;
  byte endGreen = (endColor >> 8) & 0xff;
  byte endBlue = endColor & 0xff;

  // for each step in the cross-fade
  for (int step = 0; step < 256; step++) {
    byte red = map(step, 0, 255, startRed, endRed);
    byte green = map(step, 0, 255, startGreen, endGreen);
    byte blue = map(step, 0, 255, startBlue, endBlue);
    setAllStrips(red, green, blue);
    delay(speed);
  }
}

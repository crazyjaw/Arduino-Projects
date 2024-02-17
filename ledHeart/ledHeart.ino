#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define NEOPIXEL_PIN 4 // neopixel data pin
#define HEART_PIN A5 // heart rate monitor pin
#define X 1 // just to make the patterns a little clearer to read. Dont make any variables named X though...

#define LED_COUNT 49



const boolean pattern[4][LED_COUNT] = {
{
     X,X,X,  X,X,X,
   X,X,0,0,X,0,0,X,X,
   X,0,0,0,0,0,0,0,X,
   X,0,0,0,0,0,0,0,X,
     X,0,0,0,0,0,X,
       X,0,0,0,X,
         X,0,X,
           X
  },
  {
     0,0,0,  0,0,0,
   0,0,X,X,0,X,X,0,0,
   0,X,0,0,X,0,0,X,0,
   0,X,0,0,0,0,0,X,0,
     0,X,0,0,0,X,0,
       0,X,0,X,0,
         0,X,0,
           0
  },
 {
     0,0,0,  0,0,0,
   0,0,0,0,0,0,0,0,0,
   0,0,X,X,0,X,X,0,0,
   0,0,X,X,X,X,X,0,0,
     0,0,X,X,X,0,0,
       0,0,X,0,0,
         0,0,0,
           0
  },
   {
     0,0,0,  0,0,0,
   0,0,0,0,0,0,0,0,0,
   0,0,0,X,0,X,0,0,0,
   0,0,0,X,X,X,0,0,0,
     0,0,0,X,0,0,0,
       0,0,0,0,0,
         0,0,0,
           0
  }

};


// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  heartBeat();

}

void heartBeat() {
  uint8_t ledIndex, colorVal, frameIndex;

  for (colorVal = 0; colorVal < 256; colorVal++) {
    for (frameIndex = 0; frameIndex < 4; frameIndex++) {
      for (ledIndex = 0; ledIndex < LED_COUNT; ledIndex++) {
        if ((boolean)pattern[frameIndex][ledIndex] == 1) {
          strip.setPixelColor(ledIndex, Wheel((frameIndex * 32 + colorVal) & 255));

        }
      }

    }
    strip.show();
    delay(4);
  }
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


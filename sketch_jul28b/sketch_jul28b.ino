
/* rainbow_beat
 *
 * By: Andrew Tuline
 *
 * Date: July, 2015
 * 
 * A rainbow moving back and forth using FastLED function calls with no delays or 'for' loops. Very simple, yet effective.
 *
 */


#include "FastLED.h"                                          // FastLED library.

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif
 
                                  // Number of LED's
// How many leds in your strip?
#define NUM_LEDS 60

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 4
#define CLOCK_PIN 13


// Global variables can be changed on the fly.
uint8_t max_bright = 128;                                      // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

// Palette definitions
CRGBPalette16 currentPalette;
TBlendType    currentBlending;                                // NOBLEND or LINEARBLEND

// Define variables used by the sequence.
unsigned int  brightness = 0;                       

void setup() {
  delay(1000);      
  LEDS.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  LEDS.setBrightness(128);
  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 1000);               // FastLED Power management set at 5V, 500mA
  
   currentBlending = LINEARBLEND;  
  currentPalette = PartyColors_p;
  
}




void loop() {
   
  EVERY_N_MILLIS_I(thisTimer,100) {                           // This only sets the Initial timer delay. To change this value, you need to use thisTimer.setPeriod(); You could also call it thatTimer and so on.
    uint8_t timeval =1;                     // Create/modify a variable based on the beastsin8() function.
    thisTimer.setPeriod(timeval);                             // Use that as our update timer value.
    brightness = (brightness+1) % (255-1);                       // A simple routine to just move the active LED UP the strip.
//    leds[ledLoc] = CHSV(0, 255, 255);     //

    fill_solid(leds, NUM_LEDS,  CHSV(0, 255, brightness));
  }

  fadeToBlackBy(leds, NUM_LEDS, 1);         
// 
//  EVERY_N_MILLIS_I(thisTimer,100) {                           // This only sets the Initial timer delay. To change this value, you need to use thisTimer.setPeriod(); You could also call it thatTimer and so on.
//    uint8_t timeval =10;                     // Create/modify a variable based on the beastsin8() function.
//    thisTimer.setPeriod(timeval);                             // Use that as our update timer value.
//    ledLoc = (ledLoc+1) % (NUM_LEDS-1);                       // A simple routine to just move the active LED UP the strip.
//    leds[ledLoc] = CHSV(0, 255, 255);     // Pick a slightly rotating colour from the Palette
//  }
//
//  fadeToBlackBy(leds, NUM_LEDS, 1);                           // Leave a nice comet trail behind.

  FastLED.show();
 
} // loop()

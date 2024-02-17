
#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 70

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 8
#define CLOCK_PIN 13

#define MIN 0
#define SMALL_MIN 0
#define MAX 250
#define LENGHT_OF_NIGHT (1000*60*60*13)
#define ONE_DAY (1000*60*60*24)
#define FULL_CYCLE_TIME  (1000*60*20)
int brightness = 0;
int brightnessSide = 175;
int fadeAmount = 3;
bool bounce = false;
unsigned long timeSinceStart = 0;
int minB = SMALL_MIN;
int maxB = MAX;
unsigned int  beatCount = 1;
// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(9600);
  delay(1000);
  LEDS.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip)      .setDither(true);

  LEDS.setBrightness(180);
  set_max_power_in_volts_and_milliamps(5, 1000);
}

void loop() {
  timeSinceStart = millis() % ONE_DAY;

  if(timeSinceStart  < LENGHT_OF_NIGHT){
//   
    if(timeSinceStart % FULL_CYCLE_TIME < FULL_CYCLE_TIME/2) {
     beat(); 
    } else {
     pulse(); 
    }
    
  } else {
       FastLED.clear();
       FastLED.show();
  }
}

void pulse() {
  float colorModifier =  (float)( brightness - MIN) /  (MAX - MIN);
  int augmentedColor = round(20 * colorModifier ) ;

  for (int i = NUM_LEDS - 1 ; i >= 0 ; i--) {
    leds[i] = CHSV(25, 255, 250);
    fadeToBlackBy(leds, NUM_LEDS, 20);
    FastLED.show();
    FastLED.delay(18);
  }
}
void beat () {

  if ( beatCount % 2) {
    minB = MIN;
  }
  float colorModifier =  (float)( brightness - minB) /  (maxB - minB);
  int augmentedColor = round(25 * colorModifier ) ;
  fill_solid(leds, NUM_LEDS, CHSV(augmentedColor, 255, brightness));
  FastLED.delay(25);
  FastLED.show();


  if (brightness <= minB) {
    if (beatCount % 2 == 0) {
      minB = MIN;
      FastLED.delay(700);

    } else {
      FastLED.delay(700);
      minB = SMALL_MIN;
      brightness = minB + 1;
    }
    beatCount++;

    fadeAmount = -fadeAmount ;
  }

  if (brightness >= maxB) {
    fadeAmount = -fadeAmount ;
  }

  brightness = constrain(brightness + fadeAmount, minB, maxB);

}

int fadeSection (int intensity, int startLED, int endLED) {

  if (intensity <= minB) {
    minB = MIN;
    intensity = minB + 1;
    beatCount++;
    fadeAmount = -fadeAmount ;
  }

  if (intensity >= maxB) {
    fadeAmount = -fadeAmount ;
  }

  return constrain(intensity + fadeAmount, minB, maxB);
}


int getColor (int intensity) {
  float colorModifier =  (float)( intensity - minB) /  (maxB - minB);
  return round(25 * colorModifier ) ;


}

void doubleBeat () {
  brightness = fadeSection(brightness, 0, NUM_LEDS);
  for (int i = 0; i < 40; i++) {
    leds[i] =  CHSV(getColor(brightness), 255, brightness);
  }


  brightnessSide = fadeSection(brightnessSide, 0, NUM_LEDS);
  for (int k = 40; k < NUM_LEDS; k++) {
    leds[k] =  CHSV(getColor(brightnessSide), 255, brightnessSide);
  }
  fill_solid(leds, NUM_LEDS, CHSV(getColor(brightness), 255, brightness));
  FastLED.delay(7);
  FastLED.show();
}

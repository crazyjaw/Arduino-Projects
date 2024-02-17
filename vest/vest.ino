#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 200

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 0
#define BEAT_INTERRUPT_PIN 2
#define MODE_INTERRUPT_PIN 3
#define BEAT_INPUT_RESET_TIMEOUT 2000


// Define the array of leds
CRGB leds[NUM_LEDS];
unsigned long lastBeat = 0;

bool playBeat = false;
int mode = 0;

int bpm = 100;

volatile long beatInterval = 0;
volatile int beatsInput[4];
volatile int nextBeatIndex = 0;
static unsigned long last_interrupt_time = 0;
void setup() {

  //  delay(1500);
  LEDS.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip  );
  LEDS.setBrightness(128);
}

void loop() {
  switch (mode) {
    case 0:
      doublePulseFade(true);
      break;
    case 1:
      solidPulse();
      break;
    case 2:
      cylonRainbow(true);
      break;
    case 3:
      doubleCylonRainbow(false);
      break;
    default:
      FastLED.clear();
      FastLED.show();
      break;
  }

}

void changeMode () {
  if (debounced()) {
    mode++;
    if (mode > 4) {
      mode = 0;
    }
  }
}

bool debounced() {

  unsigned long interrupt_time = millis();
  if ((interrupt_time - last_interrupt_time) > 100) {
    last_interrupt_time = interrupt_time;
   
    return false;

  } else {
     Serial.println("debounced ");
    return true;
  }
}

void setBeatInput() {

  unsigned long interrupt_time = millis();
  if (debounced()) {

    Serial.print("nextBeat: ");
    Serial.println(nextBeatIndex);
    beatsInput[nextBeatIndex] = interrupt_time;

    nextBeatIndex++;
    if (nextBeatIndex == 4) {
      updateBPM();
      resetBeats();
      Serial.println(beatInterval);

    }

    last_interrupt_time = interrupt_time;
  }
}

void updateBPM() {
  long interval = 0;
  for ( int i = 1; i < 4; i++) {
    long elapse = beatsInput[i] - beatsInput [i - 1];
    if (elapse < BEAT_INPUT_RESET_TIMEOUT && elapse > 100) {
      if (interval == 0) {
        interval = elapse;
      }
      Serial.print("elapse: ");
      Serial.println(elapse);
      interval = (interval + elapse) / 2;
    }
  }
  beatInterval = interval;
}
void resetBeats() {
  nextBeatIndex = 0;
  for ( int i = 0; i < 4; i++) {
    beatsInput[i] = 0;
  }
}

void doublePulseFade(bool withFade) {
  static uint8_t hue = 0;
  // First slide the led in one direction

  long currentTick = millis();

  if (currentTick - lastBeat >= beatInterval) {

    lastBeat = millis();

    int midPoint = NUM_LEDS / 2;
    for (int i = 0; i < NUM_LEDS / 2 + 1; i++) {
      int topPixel = constrain(midPoint + i, 0, NUM_LEDS - 1);
      int botPixel = constrain(midPoint - i, 0, NUM_LEDS);
      CHSV color = CHSV(hue++, 255, 255);
      leds[topPixel] = color;
      leds[botPixel] = color;
      if (withFade) {
        fadeToBlackBy(leds, NUM_LEDS, 2);
      }
      FastLED.show();
    }
  } else if (withFade) {

    fadeToBlackBy(leds, NUM_LEDS, 2);
    FastLED.show();

  }
}

void solidPulse() {
  static uint8_t hue = 0;
  // First slide the led in one direction

  long currentTick = millis();

  if (currentTick - lastBeat >= beatInterval) {

    lastBeat = millis();

    int midPoint = NUM_LEDS / 2;
    for (int i = 0; i < NUM_LEDS / 2 + 1; i++) {
      int topPixel = constrain(midPoint + i, 0, NUM_LEDS - 1);
      int botPixel = constrain(midPoint - i, 0, NUM_LEDS);
      CHSV color = CHSV(hue, 255, 255);
      leds[topPixel] = color;
      leds[botPixel] = color;

      FastLED.show();
    }
    hue += 47;
  }
}

void cylonRainbow(bool withFade) {
  static uint8_t hue = 0;
  // First slide the led in one direction

  for (int i = 0; i < NUM_LEDS; i++) {
    CHSV color = CHSV(hue++, 255, 255);
    leds[i] = color;
    if (withFade) {
      fadeToBlackBy(leds, NUM_LEDS, 2);
    }
    //      if( i %2 == 0) {
    FastLED.show();
    //      }
  }
  for (int i = NUM_LEDS - 1; i >= 0; i--) {
    CHSV color = CHSV(hue++, 255, 255);
    leds[i] = color;
    if (withFade) {
      fadeToBlackBy(leds, NUM_LEDS, 2);
    }
    // if( i %2 == 0) {
    FastLED.show();
    //      }
  }

}


void doubleCylonRainbow(bool withFade) {
  static uint8_t hue = 0;
  // First slide the led in one direction
  long currentTick = millis();
  if (currentTick - lastBeat >= beatInterval  ) {
    lastBeat = millis();

    for (int i = 0; i < NUM_LEDS; i++) {
      CHSV color = CHSV(hue++, 255, 255);
      leds[i] = color;
      leds[(NUM_LEDS - 1) - i] = color;
      if (i % 12 != 0 ) {
        FastLED.show();
      }
      //      FastLED.delay(1);

    }
  }

}


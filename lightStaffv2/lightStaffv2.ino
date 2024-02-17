#include "FastLED.h"
#include <math.h>
#define NUM_LEDS_PER_STRIP 54
#define NUM_LEDS NUM_LEDS_PER_STRIP*2
#define DATA_PIN 8
#define DATA_PIN2 9
#define BEAT_INTERRUPT_PIN 2
#define MODE_INTERRUPT_PIN 3
#define BEAT_INPUT_RESET_TIMEOUT 2000
#define BEAT_INPUTS_SIZE 32

FASTLED_USING_NAMESPACE
CRGB leds[NUM_LEDS];

unsigned long lastBeat = 0;

bool playBeat = false;
bool modeChanged = true;
bool wasCleared = false;
int mode = 1;

int bpm = 100;

volatile unsigned long beatInterval = 0;
volatile int beatsInput[BEAT_INPUTS_SIZE];
volatile int nextBeatIndex = 0;
static unsigned long last_interrupt_time = 0;
void setup() {
  pinMode(BEAT_INTERRUPT_PIN, INPUT_PULLUP);
  pinMode(MODE_INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BEAT_INTERRUPT_PIN), setBeatInput, RISING );
  attachInterrupt(digitalPinToInterrupt(MODE_INTERRUPT_PIN), changeMode, RISING );
  Serial.begin(9600);

  LEDS.addLeds<WS2812B, DATA_PIN2, RGB>(leds, NUM_LEDS_PER_STRIP).setCorrection( TypicalLEDStrip  );
  LEDS.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP).setCorrection( TypicalLEDStrip  );
  delay(50);
  LEDS.setBrightness(200);
  LEDS.setMaxPowerInMilliWatts(5000);
  beatInterval = 60000 / bpm;
}

void loop() {
doubleCylonRainbow(true);
FastLED.show();
//    switch (mode) {
//      case 0:
//        wasCleared = false;
//        doublePulseFade(true);
//
//        break;
//      case 1:
//        solidPulse();
//        break;
//      case 2:
//        cylonRainbow(true);
//        break;
//      case 3:
//        doubleCylonRainbow(true);
//        break;
//  
//      case 4:
//        cylonRainbow(false);
//        break;
//      case 5:
//        doubleCylonRainbow(false);
//        break;
//      case 6:
//         meteorRain(11, 64, true, 12);
//      default:
//        if (!wasCleared) {
//          Serial.println("default");
//          FastLED.clear();
//          FastLED.show();
//          wasCleared = true;
//        }
//        break;
//    }
}

void changeMode () {
  if (debounced()) {
    mode++;
    if (mode > 6) {
      mode = 0;
    }
    modeChanged = true ;
    Serial.println(mode);
  }
}

bool debounced() {

  unsigned long interrupt_time = millis();
  if ((interrupt_time - last_interrupt_time) > 100) {

    Serial.print("button Pressed:");
    Serial.print(last_interrupt_time);
    Serial.print(' ');
    Serial.print(interrupt_time);
    Serial.print(':');
    Serial.println(interrupt_time - last_interrupt_time);
    last_interrupt_time = interrupt_time;
    return true;

  } else {
    Serial.println("ignored input ");
    return false;
  }
}

unsigned long lastBeatInput = 0;
void setBeatInput() {
  unsigned long interrupt_time = millis();

  if (debounced()) {
    last_interrupt_time = interrupt_time;
    if (lastBeatInput == 0) {
      lastBeatInput = interrupt_time;
      Serial.println("start time set:" );
      return;
    }
    if (interrupt_time - lastBeatInput > BEAT_INPUT_RESET_TIMEOUT || nextBeatIndex > BEAT_INPUTS_SIZE - 1 ) {
      Serial.println("reseting beats");
      lastBeatInput = interrupt_time;
      resetBeats();
      return;
    }

    beatsInput[nextBeatIndex] = interrupt_time - lastBeatInput;
    lastBeatInput = interrupt_time;
    nextBeatIndex++;
    updateBPM();
  }
}

void updateBPM() {
  long intrval = 0;
  Serial.print("setting BPM: ");
  for ( int i = 0; i < nextBeatIndex; i++) {
    Serial.print(beatsInput[i]);
    Serial.print(',');
    intrval = intrval + beatsInput[i];
  }
  Serial.println();
  beatInterval = intrval / nextBeatIndex ;
  Serial.print("new print interval:");
  Serial.println(intrval / nextBeatIndex);
    Serial.print("BPM :");
  Serial.println(60000/beatInterval);

}
void resetBeats() {
  nextBeatIndex = 0;
  for ( int i = 0; i < BEAT_INPUTS_SIZE; i++) {
    beatsInput[i] = 0;
  }
}
int reverseMap(int index) {
  if (index < NUM_LEDS_PER_STRIP) {
    return (NUM_LEDS_PER_STRIP - 1 - index);
  }
  return index;
}
int reverseMap(int index, bool flip) {
  if (flip) {
    return reverseMap(NUM_LEDS-1-index);
  }
  return reverseMap(index);
}

void doublePulseFade(bool withFade) {
  static uint8_t hue = 0;
  // First slide the led in one direction

  long currentTick = millis();

  if (currentTick - lastBeat >= beatInterval) {

    lastBeat = millis();

    int midPoint = NUM_LEDS / 2;
    for (int i = 0; i < NUM_LEDS / 2 + 1; i++) {
      if (modeChanged) {
        modeChanged = false;
        return;
      }
      int topPixel = constrain(midPoint + i, 0, NUM_LEDS - 1);
      int botPixel = constrain(midPoint - i, 0, NUM_LEDS);
      CHSV color = CHSV(hue++, 255, 255);
      leds[reverseMap(topPixel)] = color;
      leds[reverseMap(botPixel)] = color;
      if (withFade) {
        fadeToBlackBy(leds, NUM_LEDS, 2);
      }
//      FastLED.show();
    }
  } else if (withFade) {

    fadeToBlackBy(leds, NUM_LEDS, 2);
//    FastLED.show();

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
      if (modeChanged) {
        modeChanged = false;
        return;
      }
      int topPixel = constrain(midPoint + i, 0, NUM_LEDS - 1);
      int botPixel = constrain(midPoint - i, 0, NUM_LEDS);
      CHSV color = CHSV(hue, 255, 255);
      leds[reverseMap(topPixel)] = color;
      leds[reverseMap(botPixel)] = color;

      FastLED.show();
    }
    hue += 47;
  }
}

void cylonRainbow(bool withFade) {
  static uint8_t hue = 0;
  // First slide the led in one direction

  for (int i = 0; i < NUM_LEDS; i++) {
    if (modeChanged) {
      modeChanged = false;
      return;
    }
    CHSV color = CHSV(hue++, 255, 255);
    leds[reverseMap(i)] = color;
    if (withFade) {
      fadeToBlackBy(leds, NUM_LEDS, 2);
    }
    //      if( i %2 == 0) {
    FastLED.show();
    //      }
  }
  for (int i = NUM_LEDS - 1; i >= 0; i--) {
    if (modeChanged) {
      modeChanged = false;
      return;
    }
    CHSV color = CHSV(hue++, 255, 255);
    leds[reverseMap(i)] = color;
    if (withFade) {
      fadeToBlackBy(leds, NUM_LEDS, 1);
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
      if (modeChanged) {
        modeChanged = false;
        return;
      }
      CHSV color = CHSV(hue++, 255, 255);
      leds[reverseMap(i)] = color;
      leds[reverseMap((NUM_LEDS - 1) - i)] = color;
      if (i % 12 != 0 ) {
//        FastLED.show();
      }
      //      FastLED.delay(1);

    }
  }
}
void setPixel(int Pixel, byte red, byte green, byte blue) {

  leds[reverseMap(Pixel)].r = red;
  leds[reverseMap(Pixel)].g = green;
  leds[reverseMap(Pixel)].b = blue;

}

void setAll(byte red, byte green, byte blue) {
  for (int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue);
  }
  FastLED.show();
}

void meteorRain( byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay) {
  setAll(0, 0, 0);
  uint8_t hue = random(256);
  for (int i = 0; i < NUM_LEDS + NUM_LEDS_PER_STRIP; i++) {


    // fade brightness all LEDs one step
    for (int j = 0; j < NUM_LEDS; j++) {
      if ( (random(meteorSize) >7) ) {
        fadeToBlack(j, meteorTrailDecay );
      }
    }

    // draw meteor
    for (int j = 0; j < meteorSize; j++) {
      if ( ( i - j < NUM_LEDS) && (i - j >= 0) ) {
        CHSV color;
        if(i%5 == 0){
          color= CHSV(hue++, 255, 255);
        }else {
          color= CHSV(hue, 255, 255);
        }
        
        leds[reverseMap(i - j, true)] = color;
      }
    }

//    FastLED.show();
//    delay(SpeedDelay);
  }
}

void BouncingColoredBalls(int BallCount, byte colors[][3]) {
  float Gravity = -9.81;
  int StartHeight = 1;
 
  float Height[BallCount];
  float ImpactVelocityStart = sqrt( -2 * Gravity * StartHeight );
  float ImpactVelocity[BallCount];
  float TimeSinceLastBounce[BallCount];
  int   Position[BallCount];
  long  ClockTimeSinceLastBounce[BallCount];
  float Dampening[BallCount];
 
  for (int i = 0 ; i < BallCount ; i++) {  
    ClockTimeSinceLastBounce[i] = millis();
    Height[i] = StartHeight;
    Position[i] = 0;
    ImpactVelocity[i] = ImpactVelocityStart;
    TimeSinceLastBounce[i] = 0;
    Dampening[i] = 0.90 - float(i)/pow(BallCount,2);
  }

  while (true) {
    for (int i = 0 ; i < BallCount ; i++) {
      TimeSinceLastBounce[i] =  millis() - ClockTimeSinceLastBounce[i];
      Height[i] = 0.5 * Gravity * pow( TimeSinceLastBounce[i]/1000 , 2.0 ) + ImpactVelocity[i] * TimeSinceLastBounce[i]/1000;
 
      if ( Height[i] < 0 ) {                      
        Height[i] = 0;
        ImpactVelocity[i] = Dampening[i] * ImpactVelocity[i];
        ClockTimeSinceLastBounce[i] = millis();
 
        if ( ImpactVelocity[i] < 0.01 ) {
          ImpactVelocity[i] = ImpactVelocityStart;
        }
      }
      Position[i] = round( Height[i] * (NUM_LEDS - 1) / StartHeight);
    }
 
    for (int i = 0 ; i < BallCount ; i++) {
      setPixel(Position[i],colors[i][0],colors[i][1],colors[i][2]);
    }
   
    FastLED.show();
    setAll(0,0,0);
  }
}

void fadeToBlack(int ledNo, byte fadeValue) {
  leds[ledNo].fadeToBlackBy( fadeValue );
}

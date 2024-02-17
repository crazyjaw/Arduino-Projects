#include "FastLED.h"
#include <avr/sleep.h>
#include <avr/power.h>

FASTLED_USING_NAMESPACE

// FastLED "100-lines-of-code" demo reel, showing just a few
// of the kinds of animation patterns you can quickly and easily
// compose using FastLED.
//
// This example also shows one easy way to define multiple
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif


const TProgmemPalette16 FirePallet PROGMEM =
{
        CRGB::Black,
        CRGB::Maroon,
        CRGB::Black,
        CRGB::Maroon,
    
        CRGB::DarkRed,
        CRGB::Maroon,
        CRGB::DarkRed,
    
        CRGB::DarkRed,
        CRGB::DarkRed,
        CRGB::Red,
        CRGB::Orange,
    
        CRGB::Orange,
        CRGB::Red,
        CRGB::DarkRed
    };


typedef struct  {
  CRGB *leds;
  int ledCount;
  int pulses[4];
} Feather;
//typedef struct Feather Feather;

#define f1_count  14
#define f2_count  14
#define f3_count  8
#define body_count  16
#define center_count  36

CRGB feather1[f1_count];
CRGB feather2[f2_count];
CRGB feather3[f3_count];
CRGB eye[1];
CRGB body[body_count];
CRGB center[center_count];

#define COOLING  40
#define SPARKING 120
#define FRAMES_PER_SECOND 300
#define BRIGHTNESS          60
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_STRIPS  2

#define BASE_HUE 0
CRGBPalette16 gPal;
void setup() {
  noInterrupts ();           // timed sequence follows
  sleep_enable();

  interrupts ();             // guarantees next instruction executed
  sleep_cpu ();              // sleep within 3 clock cycles of above

  for (int i = 12; i <= 25; i++) {
    pinMode(i, OUTPUT);
    digitalWrite (i, LOW);
  }
  for (byte i = 0; i <= A5; i++)
  {
    pinMode (i, OUTPUT);    // changed as per below
    digitalWrite (i, LOW);  //     ditto
  }
  delay(3000); // 3 second delay for recovery

  FastLED.addLeds<LED_TYPE, 2, COLOR_ORDER>(feather1, f1_count).setCorrection(TypicalSMD5050 );
  FastLED.addLeds<LED_TYPE, 3, COLOR_ORDER>(feather2, f2_count).setCorrection(TypicalSMD5050 );
  FastLED.addLeds<LED_TYPE, 4, COLOR_ORDER>(body, body_count).setCorrection(TypicalSMD5050 );
  FastLED.addLeds<LED_TYPE, 6, COLOR_ORDER>(eye, 1).setCorrection(TypicalSMD5050 );
  FastLED.addLeds<LED_TYPE, 5, COLOR_ORDER>(center, center_count).setCorrection(TypicalSMD5050 );
  FastLED.addLeds<LED_TYPE, 9, COLOR_ORDER>(feather3, f3_count).setCorrection(TypicalSMD5050 );
  gPal = FirePallet ;
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}

typedef struct  {
  int pos;
  int value;
  int directn;
} TwinkleLED;

uint8_t gHue = BASE_HUE;
uint8_t centerHue = 0;
int mouthPulses[4] = {-7,-7,-7,-7};
 
void loop()
{
  static Feather feathers[NUM_STRIPS] =  {
    {feather1, f1_count, { -7, -7, -7, -7}},
    {feather2, f2_count, { -7, -7, -7, -7}},
    //    {feather4, f4_count, { -7, -7, -7, -7}},
    //    {feather5, f5_count, { -7, -7, -7, -7}},
    //    {feather6, f6_count, { -7, -7, -7, -7}},
    //    {feather7, f7_count, { -7, -7, -7, -7}},
    //    {feather8, f8_count, { -7, -7, -7, -7}}
  };

  static int twinkles[3][3] =  {
    { -1, 255, -1},
    { -1, 255, -1},
    { -1, 255, -1}
  };

  for (int i = 0; i < NUM_STRIPS; i++) {
    changedTips(feathers[i].leds, feathers[i].ledCount / 2, feathers[i].pulses);
  }
  mouth(feather3, f3_count);
  rainbow(body, body_count);
  brainCase(center, center_count);
  
  CHSV color = CHSV( gHue + 128 , 255, 255 );
  eye[0] = color;
  // send the 'leds' array out to the actual LED strip
  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(1000 / FRAMES_PER_SECOND);
  EVERY_N_MILLISECONDS( 40 ) {
        
//       Fire(feather3, f3_count, 100 ,200 ,20);
  }
  EVERY_N_MILLISECONDS( 20 ) {
        gHue++;
//       Fire(feather3, f3_count, 100 ,50 ,20);

    //      gHue = beatsin16(3000, 140, 255);
    //    if(gHue > 255){
    //      gHue = 140;
    //    }
  } // slowly cycle the "base color" through the rainbow
}
#define MIN 120
#define SMALL_MIN 120
#define MAX 240
int brightness = 120;
unsigned int  beatCount = 1;
int minB = SMALL_MIN;
int maxB = MAX;
int brightnessSide = 175;
int fadeAmount = 2;

#define NUM_TWINKLES 10
int tValues[NUM_TWINKLES] = {255, 255, 255, 255, 255, 255, 255, 255, 255, 255};
int tPos[NUM_TWINKLES] = { -1, -1, -1, -1, -1,  -1, -1, -1, -1, -1};
int tDirections[NUM_TWINKLES] = { -1, -1, -1, -1, -1,  -1, -1, -1, -1, -1};

void mouth(CRGB leds[], int count) {
  for(int i = 0; i < count; i++){
    leds[i] = CHSV(gHue -128 - i*5, 255, brightness );
  }
}

void brainCase(CRGB leds[], int count) {

  if ( beatCount % 2) {
    minB = MIN;
  }
  float colorModifier =  (float)( brightness - minB) /  (maxB - minB);
  //  int augmentedColor = round(25 * colorModifier ) ;
  int augmentedColor = gHue ;
  for (int i = 0; i < count; i++) {
    CHSV color =  CHSV(augmentedColor - 38, 255, brightness - 50);
    if (i > 23) {
      color =  CHSV(augmentedColor, 255, brightness);
    }
    leds[i] =  color;
  }


  FastLED.show();
  if (brightness <= minB) {
    if (beatCount % 2 == 0) {
      minB = MIN;
    } else {
      minB = SMALL_MIN;
      brightness = minB + 1;
    }
    beatCount++;
    fadeAmount = -fadeAmount ;
  }

  if (brightness >= maxB) {
    fadeAmount = -fadeAmount ;
  }
  for (int i = 0; i < 6; i++) {
    if (tPos[i] < 0) {
      if (random(0, 100) > 90) {
        int newTwinkle = random(0, count);
        bool hasValue = false;
        for ( int k = 0; k < 6; k++) {
          if (tPos[k] == newTwinkle) {
            hasValue = true;
          }
        }
        if (!hasValue) {

          tPos[i] = newTwinkle;
          tValues[i] = 255;
          tDirections[i] = -1;
        }
      }
    } else {

      if (tValues[i] < 0) {
        tDirections[i] = 1;
        tValues[i] = 0;
      }

      if ( tPos[i] > 23) {
        leds[tPos[i]] = CHSV( augmentedColor , constrain(tValues[i], 0, 255), 255  );
      } else {
        leds[tPos[i]] = CHSV( augmentedColor - 38, constrain(tValues[i], 0, 255), 255);
      }
      tValues[i] += (tDirections[i] * 10);
      if (tValues[i] > 255 && tDirections[i] == 1) {
        tPos[i] = -1;
      }

    }
  }

  //  brightness = constrain(brightness + fadeAmount, minB, maxB);
  brightness  = 235;
}

#define PULSES  3
//int pulseLEDs[PULSES] = {-4,-4,-4,-4};
void changedTips(CRGB leds[], int count, int pulseLEDs[]) {
  int hueStep = (int)(38 / count);
  int brightnessStep = (int)(120 / count);
  for ( int i = 0; i < count; i++) {
    int pos = i;
    if (pos < count && pos >= 0 ) {
      int degrade = (int)(80 / count) * (pos) ;
      int hue = (gHue ) -  (hueStep * pos);

      CHSV color = CHSV( hue , 255, 255 - (pos * 2 ) );
      leds[pos] = color;
      leds[(count * 2) - pos - 1] = color;

    }
  }
}

void pulse(CRGB leds[], int count, int pulseLEDs[]) {

  int brightnessStep = (int)(120 / count);
  for ( int i = 0; i < count; i++) {
    leds[i].fadeToBlackBy(i * 4);
    leds[count * 2 - i - 1].fadeLightBy(i * 4);
  }

  for ( int z = 0; z < PULSES; z++) {
    if (pulseLEDs[z] > count) {
      pulseLEDs[z] = -7;
    }
    if (pulseLEDs[z] == -7) {
      if (random8() < 15) {
        pulseLEDs[z] = -6;
      }
    } else {

      //      //set behind pulse color
      //      for (int b = 0; b <= 3; b++) {
      //        int hueStep = (int)(45 / count);
      //        int pos = pulseLEDs[z] - b;
      //        if (pos >= 0) {
      //
      //          int hue = (gHue ) - (hueStep) * (pos );
      //
      //          CHSV color = CHSV( hue , 255, 225 - (pos * 2) );
      //          leds[pos] = color;
      //          leds[(count * 2) - pos - 1] = color;
      //
      //        }
      //      }
      //set pulse color
      for (int j = 0; j <= 6; j++) {
        int hueStep = (int)(38 / count);
        int pos = pulseLEDs[z] + j;
        if (pos < count && pos >= 0 ) {
          int degrade = (int)(80 / count) * (pos) ;
          int hue = (gHue ) -  (hueStep * pos);

          CHSV color = CHSV( hue , 255, 255 - (pos * 2 ) );
          leds[pos] = color;
          leds[(count * 2) - pos - 1] = color;

        }
      }

      pulseLEDs[z] = pulseLEDs[z] + 1;
    }
  }



}



int centerPos = 0;
void centerPulse(CRGB leds[], int count) {
  int hueStep = (int)(40 / 7);
  int brightnessStep = (int)(60 / 7);
  for ( int i = 0; i < count; i++) {
    //    if (centerHue > 10 && centerHue < 160 ) {
    //      centerHue = 160;
    //    }
    leds[i] = CHSV( centerHue , 200, 200 );

    centerHue = gHue;// beatsin16(15, gHue, gHue+25);
    //      centerHue++;
  }
  for (int j = 0; j < 8; j++) {
    int pos = (centerPos + j) % count;
    int pos2 = (centerPos + (count / 2) + j) % count;

    CHSV color = CHSV( centerHue, 255 - (brightnessStep * j), 255  );
    leds[pos] = color;
    leds[pos2] = color;

  }
  EVERY_N_MILLISECONDS( 35 ) {
    centerPos++;
    gHue++;
  }

  //    int pos = beatsin16(13, 0, count);
  //  leds[pos] = CHSV( 0 , 255, 255 );
  //  sinelon(leds, count);
}

void rainbow(CRGB leds[], int count)
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, count, gHue, -7);
}

void rainbowWithGlitter(CRGB leds[], int count)
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow( leds, count);
  addGlitter(leds, count, 80);
}

void addGlitter(CRGB leds[], int count, fract8 chanceOfGlitter)
{
  if ( random8() < chanceOfGlitter) {
    leds[ random16(count) ] += CRGB::White;
  }
}
//
void confetti(CRGB leds[], int count)
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, count, 10);
  fill_solid(leds, count, CHSV( gHue , 255, 255));
  int pos = random16(count);
  int numRand = random(1, 3);
  for ( int i = 0; i < numRand; i++) {
    addGlitter(leds, count, 40);
  }


  //  leds[pos] +=CRGB::White;
}


void sinelon(CRGB leds[], int count)
{
  // a colored dot sweeping back and forth, with fading trails
  //  fadeToBlackBy( leds, count, 20);
  int pos = beatsin16(13, 0, count);
  leds[pos] = CHSV( gHue, 255, 192);
}

void setPixel(CRGB leds[], int Pixel, byte red, byte green, byte blue) {

  // FastLED
  leds[Pixel].r = red;
  leds[Pixel].g = green;
  leds[Pixel].b = blue;

}



void meteorRain(CRGB leds[], int count, byte meteorSize, boolean meteorRandomDecay) {

  for (int i = 0; i < count + count; i++) {


    // fade brightness all LEDs one step
    for (int j = 0; j < count; j++) {
      if ( (!meteorRandomDecay) || (random(10) > 5) ) {
        leds[j].fadeToBlackBy(10);

      }
    }

    // draw meteor
    for (int j = 0; j < meteorSize; j++) {
      if ( ( i - j < count) && (i - j >= 0) ) {
        //        setPixel(leds, i-j, red, green, blue);
        leds[i - j] = CHSV(gHue, 255, 255);
      }
    }


    //    delay(SpeedDelay);
  }
}

bool gReverseDirection = false;


void Fire(CRGB leds[], int count, int Cooling, int Sparking, int SpeedDelay) {
// Array of temperature readings at each simulation cell
  static byte heat[8];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < count; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((Cooling * 10) / count) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= count - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < Sparking ) {
      int y = 0;
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < count; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8( heat[j], 240);
      CRGB color = ColorFromPalette( gPal, colorindex, 255, LINEARBLEND);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (count-1) - j;
      } else {
        pixelnumber = j;
      }
      leds[pixelnumber] = color;
    }
}


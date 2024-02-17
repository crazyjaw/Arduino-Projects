//#include "FastLED.h"
//
//FASTLED_USING_NAMESPACE
//
//// FastLED "100-lines-of-code" demo reel, showing just a few
//// of the kinds of animation patterns you can quickly and easily
//// compose using FastLED.
////
//// This example also shows one easy way to define multiple
//// animations patterns and have them automatically rotate.
////
//// -Mark Kriegsman, December 2014
//
//#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
//#warning "Requires FastLED 3.1 or later; check github for latest code."
//#endif
//
//
//typedef struct  {
//  CRGB *leds;
//  int ledCount;
//  int pulses[4];
//} Feather;
////typedef struct Feather Feather;
//
//#define f1_count  14
//#define f2_count  14
//#define f3_count  16
//#define f4_count  18
//#define f5_count  22
//#define f6_count  24
//#define f7_count  24
//#define f8_count  26
//#define center_count  26
//
//CRGB feather1[f1_count];
//CRGB feather2[f2_count];
//CRGB feather3[f3_count];
//CRGB feather4[f4_count];
//CRGB feather5[f5_count];
//CRGB feather6[f6_count];
//CRGB feather7[f7_count];
//CRGB feather8[f8_count];
//CRGB center[center_count];
//
//#define COOLING  40
//#define SPARKING 120
//#define FRAMES_PER_SECOND 40
//#define BRIGHTNESS          80
//#define LED_TYPE    WS2812B
//#define COLOR_ORDER GRB
//#define NUM_STRIPS  9
//
//void setup() {
//  delay(3000); // 3 second delay for recovery
//
//  FastLED.addLeds<LED_TYPE, 11, COLOR_ORDER>(feather1, f1_count).setCorrection(TypicalLEDStrip);
//  FastLED.addLeds<LED_TYPE, 10, COLOR_ORDER>(feather2, f2_count).setCorrection(TypicalLEDStrip);
//  FastLED.addLeds<LED_TYPE, 9, COLOR_ORDER>(feather3, f3_count).setCorrection(TypicalLEDStrip);
//  FastLED.addLeds<LED_TYPE, 4, COLOR_ORDER>(feather4, f4_count).setCorrection(TypicalLEDStrip);
//  FastLED.addLeds<LED_TYPE, 3, COLOR_ORDER>(feather5, f5_count).setCorrection(TypicalLEDStrip);
//  FastLED.addLeds<LED_TYPE, 2, COLOR_ORDER>(feather6, f6_count).setCorrection(TypicalLEDStrip);
//  FastLED.addLeds<LED_TYPE, 1, COLOR_ORDER>(feather7, f7_count).setCorrection(TypicalLEDStrip);
//  FastLED.addLeds<LED_TYPE, 0, COLOR_ORDER>(feather8, f8_count).setCorrection(TypicalLEDStrip);
//  FastLED.addLeds<LED_TYPE, 5, COLOR_ORDER>(center, center_count).setCorrection(TypicalLEDStrip);
//
//  // set master brightness control
//  FastLED.setBrightness(BRIGHTNESS);
//}
//
//uint8_t gHue = 200;
//uint8_t centerHue = 0;
//
//void loop()
//{
//  static Feather feathers[NUM_STRIPS] =  {
//    {feather1, f1_count, { -4, -4, -4, -4}},
//    {feather2, f2_count, { -4, -4, -4, -4}},
//    {feather3, f3_count, { -4, -4, -4, -4}},
//    {feather4, f4_count, { -4, -4, -4, -4}},
//    {feather5, f5_count, { -4, -4, -4, -4}},
//    {feather6, f6_count, { -4, -4, -4, -4}},
//    {feather7, f7_count, { -4, -4, -4, -4}},
//    {feather8, f8_count, { -4, -4, -4, -4}}
//  };
//
//
//  for (int i = 0; i < 8; i++) {
//    pulse(feathers[i].leds, feathers[i].ledCount / 2, feathers[i].pulses);
//  }
//  centerPulse(center, center_count);
//
//  // send the 'leds' array out to the actual LED strip
//  FastLED.show();
//  // insert a delay to keep the framerate modest
//  FastLED.delay(1000 / FRAMES_PER_SECOND);
//
//  // do some periodic updates
//  EVERY_N_MILLISECONDS( 20 ) {
//    //    gHue++;
//    //      gHue = beatsin16(3000, 140, 255);
//    //    if(gHue > 255){
//    //      gHue = 140;
//    //    }
//  } // slowly cycle the "base color" through the rainbow
//}
//
//#define PULSES  4
////int pulseLEDs[PULSES] = {-4,-4,-4,-4};
//void pulse(CRGB leds[], int count, int pulseLEDs[]) {
//
//  int brightnessStep = (int)(120 / count);
//  for ( int i = 0; i < count; i++) {
//    leds[i].fadeToBlackBy(i * 4);
//    leds[count * 2 - i - 1].fadeLightBy(i * 4);
//  }
//
//  for ( int z = 0; z < PULSES; z++) {
//    if (pulseLEDs[z] > count) {
//      pulseLEDs[z] = -4;
//    }
//    if (pulseLEDs[z] == -4) {
//      if (random8() < 15) {
//        pulseLEDs[z] = -3;
//      }
//    } else {
//
//      //set behind pulse color
//      for (int b = 0; b <= 3; b++) {
//        int hueStep = (int)(45 / count);
//        int pos = pulseLEDs[z] - b;
//        if (pos >= 0) {
//
//          int hue = (gHue ) - (hueStep) * (pos );
//
//          CHSV color = CHSV( hue , 255, 255 - (pos * 2) );
//          leds[pos] = color;
//          leds[(count * 2) - pos - 1] = color;
//
//        }
//      }
//      //set pulse color
//      for (int j = 0; j <= 2; j++) {
//        int hueStep = (int)(38 / count);
//        int pos = pulseLEDs[z] + j;
//        if (pos < count && pos >= 0 ) {
//          int degrade = (int)(80 / count) * (pos) ;
//          int hue = (gHue - 10 ) -  (hueStep * pos);
//
//          CHSV color = CHSV( hue , 175 + (degrade), 255 );
//          leds[pos] = color;
//          leds[(count * 2) - pos - 1] = color;
//
//        }
//      }
//
//      pulseLEDs[z] = pulseLEDs[z] + 1;
//    }
//  }
//
//
//
//}
//
//int centerPos = 0;
//void centerPulse(CRGB leds[], int count) {
//  for ( int i = 0; i < count; i++) {
////    if (centerHue > 10 && centerHue < 160 ) {
////      centerHue = 160;
////    }
//    leds[i] = CHSV( centerHue , 255, 255 );
//    
//      centerHue = beatsin16(15, 160, 200);
////      centerHue++;
//  }
////    int pos = beatsin16(13, 0, count);
////  leds[pos] = CHSV( 0 , 255, 255 );
//  //  sinelon(leds, count);
//}
//
//void rainbow(CRGB leds[], int count)
//{
//  // FastLED's built-in rainbow generator
//  fill_rainbow( leds, count, gHue, 7);
//}
//
//void rainbowWithGlitter(CRGB leds[], int count)
//{
//  // built-in FastLED rainbow, plus some random sparkly glitter
//  rainbow( leds, count);
//  addGlitter(leds, count, 80);
//}
//
//void addGlitter(CRGB leds[], int count, fract8 chanceOfGlitter)
//{
//  if ( random8() < chanceOfGlitter) {
//    leds[ random16(count) ] += CRGB::White;
//  }
//}
////
//void confetti(CRGB leds[], int count)
//{
//  // random colored speckles that blink in and fade smoothly
//  fadeToBlackBy( leds, count, 10);
//  fill_solid(leds, count, CHSV( gHue , 255, 255));
//  int pos = random16(count);
//  addGlitter(leds, count, 10);
//  //  leds[pos] +=CRGB::White;
//}
//
//
//void sinelon(CRGB leds[], int count)
//{
//  // a colored dot sweeping back and forth, with fading trails
//  //  fadeToBlackBy( leds, count, 20);
//  int pos = beatsin16(13, 0, count);
//  leds[pos] = CHSV( gHue, 255, 192);
//}
//
//void setPixel(CRGB leds[], int Pixel, byte red, byte green, byte blue) {
//
//  // FastLED
//  leds[Pixel].r = red;
//  leds[Pixel].g = green;
//  leds[Pixel].b = blue;
//
//}
//
//
//
//void meteorRain(CRGB leds[], int count, byte meteorSize, boolean meteorRandomDecay) {
//
//  for (int i = 0; i < count + count; i++) {
//
//
//    // fade brightness all LEDs one step
//    for (int j = 0; j < count; j++) {
//      if ( (!meteorRandomDecay) || (random(10) > 5) ) {
//        leds[j].fadeToBlackBy(10);
//
//      }
//    }
//
//    // draw meteor
//    for (int j = 0; j < meteorSize; j++) {
//      if ( ( i - j < count) && (i - j >= 0) ) {
//        //        setPixel(leds, i-j, red, green, blue);
//        leds[i - j] = CHSV(gHue, 255, 255);
//      }
//    }
//
//
//    //    delay(SpeedDelay);
//  }
//}
//
//
//void Fire(CRGB leds[], int count, int Cooling, int Sparking, int SpeedDelay) {
//  static byte heat[13];
//  int cooldown;
//
//  // Step 1.  Cool down every cell a little
//  for ( int i = 0; i < count; i++) {
//    cooldown = random(0, ((Cooling * 10) / count) + 2);
//
//    if (cooldown > heat[i]) {
//      heat[i] = 0;
//    } else {
//      heat[i] = heat[i] - cooldown;
//    }
//  }
//
//  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
//  for ( int k = count - 1; k >= 2; k--) {
//    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
//  }
//
//  // Step 3.  Randomly ignite new 'sparks' near the bottom
//  if ( random(255) < Sparking ) {
//    int y = random(1);
//    heat[y] = heat[y] + random(160, 200);
//    //heat[y] = random(160,255);
//  }
//
//  // Step 4.  Convert heat to LED colors
//  for ( int j = 0; j < count; j++) {
//    setPixelHeatColor(leds, j, heat[j] );
//  }
//
//  FastLED.show();
//  //  delay(SpeedDelay);
//}
//
//void setPixelHeatColor (CRGB leds[], int Pixel, byte temperature) {
//  //   Scale 'heat' down from 0-255 to 0-191
//  //  int temp = (int) temperature;
//
//  byte t192 = round((temperature / 255.0) * 191);
//  // calculate ramp up from
//  byte heatramp = t192 & 0x3F; // 0..63
//  heatramp <<= 2; // scale up to 0..252
//
//  // figure out which third of the spectrum we're in:
//  if ( t192 > 0x80) {                    // hottest
//    leds[Pixel] = CHSV( map(heatramp, 0, 255, 25, 40), 255, 255);
//  } else if ( t192 > 0x40 ) {            // middle
//    leds[Pixel] = CHSV( map(heatramp, 0, 255, 10, 32), 255, 200);
//  } else {                               // coolest
//    leds[Pixel] = CHSV( map(heatramp, 0, 255, 0, 26), 255, 80);
//  }
//}
//
//


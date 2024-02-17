#include <Adafruit_NeoPixel.h>
 
#define PIN 0
 
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(200, PIN, NEO_GRB + NEO_KHZ800);
 
// IMPORTANT: Avoid connecting on a live circuit...if you must, connect GND first.
 
void setup() {
  strip.begin();
  strip.setBrightness(50); //adjust brightness here
  strip.show(); // Initialize all pixels to 'off'
}
 
void loop() {
  rainbowCycle(2);
//  twinkleRand(5,strip.Color(255,255,255),strip.Color(255, 0, 100),90);
//simpleWave(0.3,32,10);
}
 
 
// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
 
  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

 // number, twinkle color, background color, delay
// twinkleRand(5,strip.Color(255,255,255),strip.Color(255, 0, 100),90);
// twinkle random number of pixels
void twinkleRand(int num,uint32_t c,uint32_t bg,int wait) {
	// set background
	 stripSet(bg,0);
	 // for each num
	 for (int i=0; i<num; i++) {
	   strip.setPixelColor(random(strip.numPixels()),c);
	 }
	strip.show();
	delay(wait);
}
 
 // quickly set the entire strip a color
void stripSet(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
  }
  // move the show outside the loop
  strip.show();
  delay(wait);
}
 
  // very simple wave: velocity, cycles,delay between frames
 // simpleWave(0.03,5,10);
void simpleWave(float rate,int cycles, int wait) {
   float pos=0.0;
  // cycle through x times
  for(int x=0; x<(strip.numPixels()*cycles); x++)
  {
      pos = pos+rate;
      for(int i=0; i<strip.numPixels(); i++) {
        // sine wave, 3 offset waves make a rainbow!
        float level = sin(i+pos) * 127 + 128;
        // set color level 
        strip.setPixelColor(i,(int)level,0,0);
      }
         strip.show();
         delay(wait);
  }
}  

 
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

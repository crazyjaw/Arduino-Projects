// Simple NeoPixel test.  Lights just a few pixels at a time so a
// 1m strip can safely be powered from Arduino 5V pin.  Arduino
// may nonetheless hiccup when LEDs are first connected and not
// accept code.  So upload code first, unplug USB, connect pixels
// to GND FIRST, then +5V and digital pin 6, then re-plug USB.
// A working strip will show a few pixels moving down the line,
// cycling between red, green and blue.  If you get no response,
// might be connected to wrong end of strip (the end wires, if
// any, are no indication -- look instead for the data direction
// arrows printed on the strip).
 
#include <Adafruit_NeoPixel.h>
 
#define PIN 4
#define N_LEDS 5
 
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);
 
void setup() {
  strip.begin();
}
 
void loop() {
  pulse();
}


void pulse(){
  
    for(int i=20; i<255; i++) {
             
      for(int j=0; j < N_LEDS-1; j++){
        strip.setPixelColor(j, strip.Color(i-20, i-20, i));

      } 
      strip.show();
      delay(10);
  }
   for(int i=255; i>20; i--) {
      for(int j=0; j < N_LEDS-1; j++){
        strip.setPixelColor(j, strip.Color(i-20, i-20, i));
      }
      strip.show();
      delay(10);
  }
}


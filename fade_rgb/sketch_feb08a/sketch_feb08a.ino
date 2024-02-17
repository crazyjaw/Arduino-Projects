#include "avr/interrupt.h"; 
 #include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define PIN 0

volatile uint32_t value = 0;
volatile int lastEncoded = 0;
 
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
 
  // set pins 3 and 4 to input
  // and enable pullup resisters
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
 
  GIMSK = 0b00100000;       // Enable pin change interrupts
  PCMSK =  0b00011000;      // Enable pin change interrupt for PB3 and PB4
  sei();                    // Turn on interrupts
}
 
void loop() { 
}

void rainbow() {
  
    for(int i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+value) & 255));
    }
    strip.show();
  
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}
 
// This is the ISR that is called on each interrupt
// Taken from http://bildr.org/2012/08/rotary-encoder-arduino/
ISR(PCINT0_vect) {
  int MSB = digitalRead(3); //MSB = most significant bit
  int LSB = digitalRead(4); //LSB = least significant bit
 
  int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value
 
  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011)
    value++;
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000)
    value--;
 
  lastEncoded = encoded; //store this value for next time
 
  if (value <= 0)
    value = 255;
  if (value >= 255)
    value = 0;
  rainbow();
}



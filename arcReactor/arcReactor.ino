//NeoPixel LED Digital Strip Cylon Eye v1.10 Created by EternalCore
#include <Adafruit_NeoPixel.h>

//Settings:
#define PIN 0 //The Pin out your Neopixel DIN strip/stick is connected to (Default is 6)
#define TPIXEL 60 //The total amount of pixel's/led's in your connected strip/stick (Default is 60)
int wait_T=80; //This is the delay between moving back and forth and per pixel
int PixelCount=60; //Set this to the AMOUNT of Led's/Pixels you have or want to use on your strip And It can be used to tell where to Stop then return the eye at in the strip
int Pixel_Start_End=0; //Set this to where you want it to Start/End at
boolean UsingBar = false; //Set this to true If you are using the 8x1 Neopixel Bar Or you want to only use 3 leds for the scanner. 
  int centerVal = 255;
boolean flip = true;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(TPIXEL, PIN, NEO_GRB + NEO_KHZ800); //Standered Strip function

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  //Serial.begin(9600); //Used For pixel Count Debugging
}
void loop() {
//  for(int i = 0; i < 50; i++){
//rotate();    
//  }
  for(int i = 0; i < 10; i++){
pulse();    
  }

  



    
//CylonEyeUp(strip.Color(200,200,255), strip.Color(100,100,150), strip.Color(20,20,40), wait_T, PixelCount-1, Pixel_Start_End);

}

void rotate(){

  for(int i = 0; i < 10; i++){
      if(centerVal <=20){

        flip = false;
      }
      if(centerVal >= 250){
         flip = true;
      }
      if(flip){
        centerVal = centerVal-5;        
      }else{
        centerVal = centerVal + 5;   
      }

      strip.setPixelColor(10, strip.Color(0,0,centerVal));
      int pix1 = i %10;
      int pix2 = (i +1) %10;
      int pix3 = (i+2) %10;
      
      int k = i+5;
      int pix4 = k %10;
      int pix5 = (k +1) %10;
      int pix6 = (k+2) %10;


      for(int j = 0; j < PixelCount - 1; j++){
          strip.setPixelColor(j, strip.Color(0, 0,20));
      }
        strip.setPixelColor(pix1, strip.Color(20, 20, 40));
       strip.setPixelColor(pix2, strip.Color(100,100,150));
       strip.setPixelColor(pix3, strip.Color(222,222,222));
       
             strip.setPixelColor(pix4, strip.Color(20, 20, 40));
       strip.setPixelColor(pix5, strip.Color(100,100,150));
       strip.setPixelColor(pix6, strip.Color(222,222,222));
      
      strip.show();
      
      delay(90);
  }
  
}

void pulse(){
  
    for(int i=20; i<255; i++) {
             
      for(int j=0; j < PixelCount-1; j++){
        strip.setPixelColor(j, strip.Color(i-20, i-20, i));

      } 
      strip.setPixelColor(10, strip.Color(254,i,i));
      

      strip.show();
      delay(10);
  }
   for(int i=255; i>20; i--) {
      for(int j=0; j < PixelCount-1; j++){
        strip.setPixelColor(j, strip.Color(i-20, i-20, i));
      }
      strip.setPixelColor(10, strip.Color(254,i,i));
      
              strip.show();
      delay(10);
  }
}

void CylonEyeUp(uint32_t Co, uint32_t Ct, uint32_t Ctt, uint8_t Delay, int TotalPixels, int pStart) {
  strip.setPixelColor(10, strip.Color(254,255,255));
  for(int i=pStart; i<TotalPixels-1; i++) {

    if(i+1 < 10){
      strip.setPixelColor(i+1, Ct);   //Second Dot Color
    }
    strip.setPixelColor(i, Co);     //Center Dot Color
    strip.setPixelColor(i-1, Ct);   //Second Dot Color
  strip.setPixelColor(i-1, strip.Color(0,0,0)); //Clears the dots after the 3rd color


      strip.setPixelColor(i-3, strip.Color(0,0,0)); //Clears the dots after the 3rd color
    strip.show();
    //Serial.println(i); //Used For pixel Count Debugging
    delay(Delay);
  }
}
void CylonEyeDown(uint32_t Co, uint32_t Ct, uint32_t Ctt, uint8_t Delay, int TotalPixels, int pEnd) {
  for(int i=TotalPixels-1; i>pEnd; i--) {
    if(!UsingBar) { strip.setPixelColor(i-2, Ctt); } //Third Dot Color
    strip.setPixelColor(i-1, Ct);   //Second Dot Color
    strip.setPixelColor(i, Co);    //Center Dot Color
    strip.setPixelColor(i+1, Ct);  //Second Dot Color
    if(!UsingBar) { strip.setPixelColor(i+2, Ctt); } //Third Dot Color

    if(!UsingBar) { 
      strip.setPixelColor(i+3, strip.Color(0,0,0)); //Clears the dots after the 3rd color
    } else {
      strip.setPixelColor(i+2, strip.Color(0,0,0)); //Clears the dots after the 2rd color
    }
    strip.show();
    //Serial.println(i); //Used For pixel Count Debugging
    delay(Delay);
  }
}


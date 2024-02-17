#include <Adafruit_NeoPixel.h>
 
#define PIN      0
#define N_LEDS 20 

int lights[7][3] = {
  {0,1,2},
  {3,4,5},
  {6,7,8},
  {9,10,11},
  {12,13,14},
  {15,16,16},
  {17,18,18}  
}; 
 
 int colors[7][3] = {
   {255,0, 0},
   {255,127, 0},
   {255,255, 0},
   {0,255, 0},
   {0,0, 255},
   {127,0, 255},
   {255,0, 255}
 };
 

 
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);
 
void setup() {
  strip.begin();
}
 
void loop() {
  for( int i = 0; i <= 20; i++){
    rotate();
  }
  for( int i = 0; i <= 20; i++){
    fill();
  }
}
 
static void fill() {
  int waveSize =4;
  for(int i = 0; i < 11; i++ ){
    int back;
      for(int k = 0; k < waveSize; k++){
        if(i <11-waveSize){
          strip.setPixelColor(lights[i][k]  , strip.Color(colors[i][0], colors[i][1], colors[i][2]));
        }
        if(i>= waveSize){
          back = i - waveSize;
          strip.setPixelColor(lights[back][k]  , 0);
        }
      }
      strip.show();
      delay(100);
  }

 
  delay(200);
}


static void rotate(){
  int cells = 7;
  for( int j = 0; j < cells; j++){
      for(int i = 0; i < cells; i++ ){
          int colorShift = (i + j)%(cells);
        for(int k = 0; k < 3; k++){ 
          strip.setPixelColor(lights[i][k]  , strip.Color(colors[colorShift][0], colors[colorShift][1], colors[colorShift][2]));
        }  
        // Draw new pixel
        strip.show();   
    }
    delay(100);
  }


}

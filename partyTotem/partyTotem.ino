#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

#define PIN_LEDS 3
#define NUMPIXELS 73

#define LIGHTNESS 0.05f
#define BRIGHTNESS 50

RgbColor BLACK(0, 0, 0);
RgbColor RED(BRIGHTNESS, 0, 0);
RgbColor GREEN(0, BRIGHTNESS, 0);
RgbColor BLUE(0, 0, BRIGHTNESS);
RgbColor YELLOW(BRIGHTNESS, BRIGHTNESS, 0);
RgbColor WHITE(BRIGHTNESS, BRIGHTNESS, BRIGHTNESS);

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> leds(NUMPIXELS, PIN_LEDS);

NeoPixelAnimator animations(10); // NeoPixel animation management object

#define NUM_RINGS 7
#define RING_MAX_PIXELS 15
#define RING_MIN_PIXELS 5

// all rings starts at 0 ray
byte RINGS[NUM_RINGS][RING_MAX_PIXELS] = {
  { 0, 1, 2, 3, 4},
  { 5, 6, 7, 8, 9, 10, 11, 12, 13, 14},
  { 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28},
  { 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43 },
  { 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44},
  { 67, 66, 65, 64, 63, 62, 61, 60, 59, 58},
  { 72, 71, 70, 69, 68}

};

byte RING_SIZES[NUM_RINGS] = {5, 10, 14, 15, 14, 10, 5};

unsigned long startMillis = 0;
short animation = 2;
HslColor color = BLUE;
float hue = 0;
int runTime = 8000;

void setup() {
  Serial.begin(115200);
//  initRandom();

  leds.Begin();
  leds.ClearTo(BLACK);
  leds.Show();

  animations.StartAnimation(0, 6000, rainbowAnimation);
  //  animations.StartAnimation(0, 1500, raysRainbow);
  //  animations.StartAnimation(0, 2000, ringsRainbow);
  //animations.StartAnimation(0, 500, xmasOrbAnimation);
//  animations.StartAnimation(0, 1000, ringAnimation);
//  animations.StartAnimation(0, 600, rayAnimation);
  //animations.StartAnimation(0, 100, randomAnimation);
}

void loop() {
  animations.UpdateAnimations();

  leds.Show();
  delay(10);

  if (true && (startMillis == 0 || startMillis + runTime < millis())) { 
    startMillis = millis();
    switch (animation) {
      case 0:
        runTime = 8000;
        animations.StartAnimation(0, 600, rayAnimation);

        break;
      case 1:
        runTime = 10000;
        animations.StartAnimation(0, 6000, rainbowAnimation);
        break;
      case 2:
        runTime = 16000;
        animations.StartAnimation(0, 2000, ringsRainbow);
        break;
      case 3:
        runTime = 16000;
        animations.StartAnimation(0, 2000, raysRainbow);
        break;
      case 4:
        runTime = 8000;
        animations.StartAnimation(0, 1000, ringAnimation);
        break;
      case 5:
        runTime = 5000;
        animations.StartAnimation(0, 70, randomAnimation);
        animation = -1;
        break;
    }
    animation ++;
  }
}

void randomAnimation(const AnimationParam& param) {
  float hue;
  HslColor color;

  if (param.state == AnimationState_Completed) {
    for (byte i = 0; i < NUMPIXELS; i ++) {
      hue = random(0, 1000) / 1000.0f;
      color = HslColor(hue, 1.0f, LIGHTNESS);
      leds.SetPixelColor(i, color);
    }

    animations.RestartAnimation(0);
  }
}

void rainbowAnimation(const AnimationParam& param) {
  HslColor color = HslColor(param.progress, 1.0f, LIGHTNESS);
  leds.ClearTo(color);

  if (param.state == AnimationState_Completed) {
    animations.RestartAnimation(0);
  }
}

void raysRainbow(const AnimationParam& param) {
  HslColor color;
  float hue;

  for (int i = 0; i < RING_MAX_PIXELS; i++) {
    hue =  param.progress + (float) i / (float) RING_MAX_PIXELS;
    //  hue = i / (float) RING_MAX_PIXELS;
    if (hue > 1.0f) {
      hue -= 1.0f;
    }

    color = HslColor(hue, 1.0f, LIGHTNESS);
    rayColor(i, RgbColor(color));
  }

  if (param.state == AnimationState_Completed) {
    animations.RestartAnimation(0);
  }
}

void ringsRainbow(const AnimationParam& param) {
  HslColor color;
  float hue;

  for (int i = 0; i < NUM_RINGS; i++) {
    hue = param.progress - (float) i / 32;
    if (hue > 1.0f) {
      hue -= 1.0f;
    }

    color = HslColor(hue, 1.0f, LIGHTNESS);
    ringColor(i, RgbColor(color));
  }

  if (param.state == AnimationState_Completed) {
    animations.RestartAnimation(0);
  }
}

void ringAnimation(const AnimationParam& param) {
  int index = param.progress * (NUM_RINGS * 2 );
  RgbColor color;
  float hue = 0;
  
  hue +=  param.progress ;
  color = RgbColor(HslColor(hue, 1.0f, LIGHTNESS));
  leds.ClearTo(BLACK);
  if (index < NUM_RINGS) {
    ringColor(index, color);
  }
  else {
    ringColor(NUM_RINGS - (index - NUM_RINGS), color);
  }

  if (param.state == AnimationState_Completed) {
    animations.RestartAnimation(0);
  }
}

void rayAnimation(const AnimationParam& param) {
  int index = param.progress * (RING_MAX_PIXELS );

   hue += 1 / 360.0f;
        if( hue > 1){
          hue = 0;
        }
        color = HslColor(hue, 1.0f, LIGHTNESS); 

  //  leds.ClearTo(BLACK);

rayColor(index, color);
 
  for (uint16_t pixel = 0; pixel < RING_MAX_PIXELS; pixel++) {
    RgbColor prevColor =  leds.GetPixelColor(RINGS[3][pixel]);
    RgbColor updatedColor = RgbColor::LinearBlend(
                            prevColor,
                            BLACK,
                            0.1f);
                            
    rayColor(pixel, updatedColor);
  }
   

  if (param.state == AnimationState_Completed) {
    animations.RestartAnimation(0);
      
  }
}

void rayColor(byte rayIndex, RgbColor color) {
  int pixelIndex;
  byte pixel;

  if (rayIndex >= RING_MAX_PIXELS) {
    return; // prevents out of bounds
  }

  for (byte i = 0; i < NUM_RINGS; i ++) {
    pixelIndex = round((float) RING_SIZES[i] / (float) RING_MAX_PIXELS * rayIndex);
    if ( pixelIndex > RING_SIZES[i]) {
      continue;
    }
    pixel = RINGS[i][pixelIndex];

    leds.SetPixelColor(pixel, color); // index starts from 1 (0 is stop condition)
  }
}

void ringColor(byte ringIndex, RgbColor color) {
  byte pixel;

  if (ringIndex >= NUM_RINGS) {
    return; // prevents out of bounds
  }

  for (byte i = 0; i < RING_SIZES[ringIndex]; i ++) {
    pixel = RINGS[ringIndex][i];
    
    leds.SetPixelColor(pixel, color); // index starts from 1 (0 is stop condition)
  }
}



///////////////////////////////////////////////////////


void initRandom() {
  // random works best with a seed that can use 31 bits
  // analogRead on a unconnected pin tends toward less than four bits
  uint32_t seed = analogRead(0);
  delay(1);

  for (int shifts = 3; shifts < 31; shifts += 3) {
    seed ^= analogRead(0) << shifts;
    delay(1);
  }

  // Serial.println(seed);
  randomSeed(seed);
}


RgbColor colorWheel(byte wheelPos) {
  // Input a value 0 to 255 to get a color value.
  // The colours are a transition r - g - b - back to r.
  wheelPos = 255 - wheelPos;
  if (wheelPos < 85) {
    return RgbColor(255 - wheelPos * 3, 0, wheelPos * 3);
  }
  if (wheelPos < 170) {
    wheelPos -= 85;
    return RgbColor(0, wheelPos * 3, 255 - wheelPos * 3);
  }
  wheelPos -= 170;
  return RgbColor(wheelPos * 3, 255 - wheelPos * 3, 0);
}

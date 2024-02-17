#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

#define X 1 // just to make the patterns a little clearer to read. Dont make any variables named X though...

const boolean pattern[4][49] = {
  {
     0,0,0,  0,0,0,
   0,0,0,0,0,0,0,0,0,
   0,0,0,X,0,X,0,0,0,
   0,0,0,X,X,X,0,0,0,
     0,0,0,X,0,0,0,
       0,0,0,0,0,
         0,0,0,
           0
  },
  {
     0,0,0,  0,0,0,
   0,0,0,0,0,0,0,0,0,
   0,0,X,X,0,X,X,0,0,
   0,0,X,0,0,0,X,0,0,
     0,0,X,0,X,0,0,
       0,0,X,0,0,
         0,0,0,
           0
  },
  {
     0,0,0,  0,0,0,
   0,0,X,X,0,X,X,0,0,
   0,X,0,0,X,0,0,X,0,
   0,X,0,0,0,0,0,X,0,
     0,X,0,0,0,X,0,
       0,X,0,X,0,
         0,X,0,
           0
  },
  {
     X,X,X,  X,X,X,
   X,X,0,0,X,0,0,X,X,
   X,0,0,0,0,0,0,0,X,
   X,0,0,0,0,0,0,0,X,
     X,0,0,0,0,0,X,
       X,0,0,0,X,
         X,0,X,
           X
  }
};


const uint16_t PixelCount = 49; // make sure to set this to the number of pixels in your strip
const uint8_t PixelPin = 3;  // make sure to set this to the correct pin, ignored for Esp8266
const RgbColor CylonEyeColor(HtmlColor(0x7f0000));

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);
// for esp8266 omit the pin
//NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount);

NeoPixelAnimator animations(2); // only ever need 2 animations

uint16_t lastPixel = 0; // track the eye position
int8_t moveDir = -1; // track the direction of movement

// uncomment one of the lines below to see the effects of
// changing the ease function on the movement animation
AnimEaseFunction moveEase =
      NeoEase::Linear;
//      NeoEase::QuadraticInOut;
//      NeoEase::CubicInOut;
//        NeoEase::QuarticInOut;
//      NeoEase::QuinticInOut;
//      NeoEase::SinusoidalInOut;
//      NeoEase::ExponentialInOut;
//      NeoEase::CircularInOut;

void FadeAll(uint8_t darkenBy)
{
    RgbColor color;
    for (uint16_t indexPixel = 0; indexPixel < strip.PixelCount(); indexPixel++)
    {
        color = strip.GetPixelColor(indexPixel);
        color.Darken(darkenBy);
        strip.SetPixelColor(indexPixel, color);
    }
}

void FadeAnimUpdate(const AnimationParam& param)
{
    if (param.state == AnimationState_Completed)
    {
     
        animations.RestartAnimation(param.index);
    }
}

void idlePattern(const AnimationParam& param) {
  uint8_t ledIndex, colorVal, frameIndex;
  float hue;
  float progress = moveEase(param.progress);

    for (frameIndex = 0; frameIndex < 4; frameIndex++) {
      for (ledIndex = 0; ledIndex < 49; ledIndex++) {
        if ((boolean)pattern[frameIndex][ledIndex] == 1) {
          
//          strip.setPixelColor(ledIndex, Wheel((frameIndex * 32 + colorVal) & 255));

            if (moveDir > 0){
              hue = 0.94f + progress + (0.05f*frameIndex);
              
            }else{
              hue = 1.136f - ( progress - (0.05f*frameIndex));
            }
            RgbColor color = RgbColor(HsbColor( fmod (hue , 1) , 1.0f, 0.12f));
            strip.SetPixelColor(ledIndex, color);
//          strip.setPixelColor(ledIndex, CylonEyeColor);
        }
      }
    }
    if (param.state == AnimationState_Completed)
    {
        // reverse direction of movement
//        moveDir *= -1;

        // done, time to restart this position tracking animation/timer
        animations.RestartAnimation(param.index);
    }
    //strip.show();
//    delay(4);
  
}

void MoveAnimUpdate(const AnimationParam& param)
{
    // apply the movement animation curve
    float progress = moveEase(param.progress);

    // use the curved progress to calculate the pixel to effect
    uint16_t nextPixel;
    if (moveDir > 0)
    {
        nextPixel = progress * PixelCount;
    }
    else
    {
        nextPixel = (1.0f - progress) * PixelCount;
    }

    // if progress moves fast enough, we may move more than
    // one pixel, so we update all between the calculated and
    // the last
    if (lastPixel != nextPixel)
    {
        for (uint16_t i = lastPixel + moveDir; i != nextPixel; i += moveDir)
        {
            strip.SetPixelColor(i, CylonEyeColor);
        }
    }
    strip.SetPixelColor(nextPixel, CylonEyeColor);

    lastPixel = nextPixel;

    if (param.state == AnimationState_Completed)
    {
        // reverse direction of movement
        moveDir *= -1;

        // done, time to restart this position tracking animation/timer
        animations.RestartAnimation(param.index);
    }
}

void SetupAnimations()
{
    // fade all pixels providing a tail that is longer the faster
    // the pixel moves.
//    animations.StartAnimation(0, 5, FadeAnimUpdate);

    // take several seconds to move eye fron one side to the other
    animations.StartAnimation(1, 4000, idlePattern);
}

void setup()
{
    strip.Begin();
    strip.Show();

    SetupAnimations();
}

void loop()
{
    // this is all that is needed to keep it running
    // and avoiding using delay() is always a good thing for
    // any timing related routines
    animations.UpdateAnimations();
    strip.Show();
}

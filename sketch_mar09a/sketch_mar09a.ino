/*
   Example for how to use SinricPro Blinds device

   If you encounter any issues:
   - check the readme.md at https://github.com/sinricpro/esp8266-esp32-sdk/blob/master/README.md
   - ensure all dependent libraries are installed
     - see https://github.com/sinricpro/esp8266-esp32-sdk/blob/master/README.md#arduinoide
     - see https://github.com/sinricpro/esp8266-esp32-sdk/blob/master/README.md#dependencies
   - open serial monitor and check whats happening
   - check full user documentation at https://sinricpro.github.io/esp8266-esp32-sdk
   - visit https://github.com/sinricpro/esp8266-esp32-sdk/issues and check for existing issues or open a new one
*/

// Uncomment the following line to enable serial debug output
#define ENABLE_DEBUG

#ifdef ENABLE_DEBUG
#define DEBUG_ESP_PORT Serial
#define NODEBUG_WEBSOCKETS
#define NDEBUG
#endif

#include <Arduino.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#endif
#ifdef ESP32
#include <WiFi.h>
#endif

#include "SinricPro.h"
#include "SinricProBlinds.h"

#include <AccelStepper.h>
#define stepPin D1
#define dirPin D2
#define enablePin D6
#define motorInterfaceType 1
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

#define WIFI_SSID         "passwordIsSausages"
#define WIFI_PASS         "a24001558"
#define APP_KEY           "cfbf0ec9-348b-44ee-bb42-940eea6fb4e4"      // Should look like "de0bxxxx-1x3x-4x3x-ax2x-5dabxxxxxxxx"
#define APP_SECRET        "e945c768-ac7d-4f51-9c7b-03f9a906b818-13169233-e7bf-4494-956c-b491a6f6c742"   // Should look like "5f36xxxx-x3x7-4x3x-xexe-e86724a9xxxx-4c4axxxx-3x3x-x5xe-x9x3-333d65xxxxxx"
#define BLINDS_ID         "5e9d40f9e0c04945f6e46bda"    // Should look like "5dc1564130xxxxxxxxxxxxxx"
#define BAUD_RATE         9600                // Change baudrate to your need

int maxSpan = 4500;
int blindsPosition = 0;
bool powerState = false;

bool onPowerState(const String &deviceId, bool &state) {
  Serial.printf("Device %s power turned %s \r\n", deviceId.c_str(), state ? "on" : "off");
  powerState = state;
  return true; // request handled properly
}

bool onSetPosition(const String &deviceId, int &position) {
  Serial.printf("Device %s set position to %d\r\n", deviceId.c_str(), position);
  int adjustedPosition;
  if (position == 0) {
    adjustedPosition = 0;
  } else {
    adjustedPosition = (int) -maxSpan * (100 / position);
  }

  Serial.printf("setting adjusted position %d\r\n", adjustedPosition);
  stepper.moveTo(adjustedPosition);
  return true; // request handled properly
}

bool onAdjustPosition(const String &deviceId, int &positionDelta) {
  blindsPosition += positionDelta;
  Serial.printf("Device %s position changed about %i to %d\r\n", deviceId.c_str(), positionDelta, blindsPosition);
  positionDelta = blindsPosition; // calculate and return absolute position
  return true; // request handled properly
}


// setup function for WiFi connection
void setupWiFi() {
  Serial.printf("\r\n[Wifi]: Connecting");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf(".");
    delay(250);
  }
  IPAddress localIP = WiFi.localIP();
  Serial.printf("connected!\r\n[WiFi]: IP-Address is %d.%d.%d.%d\r\n", localIP[0], localIP[1], localIP[2], localIP[3]);
}

void setupSinricPro() {
  // get a new Blinds device from SinricPro
  SinricProBlinds &myBlinds = SinricPro[BLINDS_ID];
  myBlinds.onPowerState(onPowerState);
  myBlinds.onSetPosition(onSetPosition);
  myBlinds.onAdjustPosition(onAdjustPosition);

  // setup SinricPro
  SinricPro.onConnected([]() {
    Serial.printf("Connected to SinricPro\r\n");
  });
  SinricPro.onDisconnected([]() {
    Serial.printf("Disconnected from SinricPro\r\n");
  });
  SinricPro.begin(APP_KEY, APP_SECRET);
}

// main setup function
void setup() {
  Serial.begin(BAUD_RATE); Serial.printf("\r\n\r\n");

    stepper.setEnablePin(enablePin);
  stepper.setMaxSpeed(1250);
  stepper.setAcceleration(2400);

  setupWiFi();
  setupSinricPro();
}

void loop() {

  int remainingDistance = stepper.distanceToGo();
  if (remainingDistance != 0) {
    stepper.disableOutputs();
    //      Serial.printf("distanceToGo %i, currentDistance %u\r\n",remainingDistance, stepper.currentPosition());
    stepper.run();
  } else {
    //      Serial.printf("handle \r\n");
        stepper.enableOutputs();
    SinricPro.handle();
  }



}

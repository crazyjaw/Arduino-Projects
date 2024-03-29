/***************************************************
Copyright (c) 2017 Luis Llamas
(www.luisllamas.es)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/

#include "AsyncTimerLib.h"

#define DEBUG(a) Serial.print(millis()); Serial.print(": "); Serial.println(a);

AsyncTimer timer1(2000, true, []() { DEBUG("Expired"); });

void setup()
{
	Serial.begin(9600);
	Serial.println("Starting");

	timer1.Start();
}

void loop()
{
	timer1.Update();
}

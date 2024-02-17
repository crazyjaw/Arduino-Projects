// InMoov Finger Starter Sketch
// Controlling a servo position with different timings and speeds. 
// by Gaël Langevin <inmoov.blogspot.com> 

#include <Servo.h>

Servo servoindex;         // Define index servo

int pos = 0;    // variable to store the servo position 

void setup() { 
  servoindex.attach(3);  // Set index servo to digital pin 3

} 

void loop() {            // Loop through motion test  
  handopen();            // Open finger to 0 position
  delay(5000);           // Stays in position for 2 seconds
       // Grab has self timing included and goes from 0 to 180 and from 180 to 0 position
  handclose();           // Set finger to 180 position
  delay(5000);           // Stays in position for 5 seconds
  handrest();            // Sets finger to a "rest" position
  delay(1000);           // Stays in position for 1 seconds  
 
}

// Motion routines for handopen,handrest, handclose, grab...


void handopen() {
  servoindex.write(0);
  
}

void handrest() {
  
  servoindex.write(90);
}


void handclose() {
  servoindex.write(180);
}


void grab() { 
  for(pos = 0; pos < 180; pos+=2)    // goes from 0 degrees to 180 degrees 
  {                                
  servoindex.write(pos);             // tell servo to go to position in variable 'pos'
    delay(50);                       // waits 15ms for the servo to reach the position 
  }
  for(pos =180; pos>=0; pos-=1)      // goes from 180 degrees to 0 degrees 
  {                                
    servoindex.write(pos);           // tell servo to go to position in variable 'pos' 
    delay(25);                        // waits 5ms for the servo to reach the position 
  } 
                                
 } 
   
 
  



  

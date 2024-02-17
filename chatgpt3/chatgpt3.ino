#include <Servo.h>

// Number of fingers
const int NUM_FINGERS = 5;

// Servo motor pins
const int servoPins[NUM_FINGERS][2] = {
  {2, 3}, // Finger 1 servo pins
  {4, 5}, // Finger 2 servo pins
  {6, 7}, // Finger 3 servo pins
  {8, 9}, // Finger 4 servo pins
  {10, 11} // Finger 5 servo pins
};

// Finger class definition
class Finger {
public:
  Finger(int servoPin1, int servoPin2, float offset, int speed);
  void update();
private:
  Servo servo1;
  Servo servo2;
  float angle;
  float offset;
  int speed;
};

// Finger class constructor
Finger::Finger(int servoPin1, int servoPin2, float offset, int speed) {
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
  angle = 0;
  this->offset = offset;
  this->speed = speed;
}

// Finger class update method
void Finger::update() {
  float radian = radians(angle);
  int x = 90 + int(cos(radian) * 30); // X coordinate of arc
  int y = 90 + int(sin(radian + offset) * 30); // Y coordinate of arc
  servo1.write(x);
  servo2.write(y);
  angle += speed;
  if (angle > 360) {
    angle = 0;
  }
}

// Array of Finger objects
Finger fingers[NUM_FINGERS] = {
  Finger(servoPins[0][0], servoPins[0][1], 0, 1), // Finger 1
  Finger(servoPins[1][0], servoPins[1][1], 45, 2), // Finger 2
  Finger(servoPins[2][0], servoPins[2][1], 90, 3), // Finger 3
  Finger(servoPins[3][0], servoPins[3][1], 135, 4), // Finger 4
  Finger(servoPins[4][0], servoPins[4][1], 180, 5) // Finger 5
};

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
}

void loop() {
  // Update all fingers
  for (int i = 0; i < NUM_FINGERS; i++) {
    fingers[i].update();
  }
  
  // Delay for smooth motion
  delay(20);
}

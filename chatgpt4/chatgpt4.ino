#include <Servo.h>

class Finger {
  public:
    Finger(int servo1Pin, int servo2Pin) {
      servo1.attach(servo1Pin);
      servo2.attach(servo2Pin);
    }

    void move(int minAngle, int maxAngle, int moveDuration, int returnDuration) {
      int range = maxAngle - minAngle;

      // Move finger forward
      for (int pos = 0; pos <= range; pos++) {
        servo1.write(minAngle + pos);
        servo2.write(minAngle + pos);
        delay(moveDuration / range);
      }

      // Return finger to the initial position
      for (int pos = range; pos >= 0; pos--) {
        servo1.write(minAngle + pos);
        servo2.write(minAngle + pos);
        delay(returnDuration / range);
      }
    }

  private:
    Servo servo1, servo2;
};

// Initialize finger objects with servo pins
Finger finger1(2, 3);
Finger finger2(4, 5);
Finger finger3(6, 7);
Finger finger4(8, 9);
Finger finger5(10, 11);

void setup() {
  // Nothing to do here
}

void loop() {
  // Move each finger with an offset in motion
  finger1.move(30, 90, 1000, 1000);
  delay(200);
  finger2.move(30, 90, 1000, 1000);
  delay(200);
  finger3.move(30, 90, 1000, 1000);
  delay(200);
  finger4.move(30, 90, 1000, 1000);
  delay(200);
  finger5.move(30, 90, 1000, 1000);
}

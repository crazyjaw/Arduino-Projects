int blinkPin = 0;

void setup()
{
  pinMode(blinkPin, OUTPUT);
}

void loop()
{
  digitalWrite(blinkPin, HIGH);
  delay(1000);
  digitalWrite(blinkPin, LOW);
  delay(1000);
}

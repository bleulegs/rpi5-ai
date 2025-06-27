#include <Servo.h>

int potPin = A5;
int potValue = 0;
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;

void setup() {
  Serial.begin(9600);    // Initialize serial communication
  servo1.attach(6);
  servo2.attach(9);
  servo3.attach(10);
  servo4.attach(11);    // Attach servo to digital pin 9
}

void loop() {
  potValue = analogRead(potPin);             // Read the potentiometer
  int angle = map(potValue, 0, 1023, 0, 180);
  servo1.write(angle);
  servo2.write(angle);
  servo3.write(angle);
  servo4.write(angle);
  Serial.print("Potentiometer: ");
  Serial.print(potValue);
  Serial.print(" → Servo Angle: ");
  Serial.println(angle);
  delay(15);
}
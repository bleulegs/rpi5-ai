#include <Servo.h>

// Create servo objects
Servo servo1; // Servo 1 (X-axis normal direction)
Servo servo2; // Servo 2 (X-axis opposite direction)
Servo servo3; // Servo 3 (Y-axis same direction)
Servo servo4; // Servo 4 (Y-axis same direction)
Servo esc;    // Motor ESC control (throttle)

// Define joystick pins
const int joystick1XPin = A0; // Joystick 1 X-axis
const int joystick1YPin = A1; // Joystick 1 Y-axis
const int joystick2YPin = A2; // Joystick 2 Y-axis (throttle control)

// Servo angle limits
const int minServoAngle = 0;
const int maxServoAngle = 180;

// ESC throttle limits
const int minThrottle = 0;   // Minimum throttle
const int maxThrottle = 250; // Maximum throttle

// Dead zone size
const int deadZone = 50; // Dead zone range for joystick (around 512)

bool motorOn = false;

void setup() {
  // Attach servos to their respective pins
  servo1.attach(8);  // Servo 1 controlled by Joystick 1 X-axis (normal)
  servo2.attach(10); // Servo 2 controlled by Joystick 1 X-axis (opposite)
  servo3.attach(11); // Servo 3 controlled by Joystick 1 Y-axis (same)
  servo4.attach(12); // Servo 4 controlled by Joystick 1 Y-axis (same)

  pinMode(7, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

  pinMode(A3, INPUT_PULLUP);

  // Attach ESC to motor control pin
  esc.attach(6); // Motor ESC signal pin

  // Initialize servos and motor positions
  servo1.write(90);
  servo2.write(90);
  servo3.write(90);
  servo4.write(90);
  esc.write(minThrottle); // Start with the motor throttle at 0

  Serial.begin(9600); // For debugging
}

void loop() {
  // Read joystick 1 values
  int joystick1X = analogRead(joystick1XPin); // X-axis value (0 to 1023)
  int joystick1Y = analogRead(joystick1YPin); // Y-axis value (0 to 1023)
  int motorbtn = digitalRead(A3); // Y-axis value (0 to 1023)

  if (motorbtn == HIGH) {
    motorOn = false;
  } else {
    motorOn = true;
  }

  if (motorOn) {
    digitalWrite(5, HIGH);
  } else {
    digitalWrite(5, LOW);
  }

  if (motorOn) {
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    delay(2000);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    delay(1000);
  } else {
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
  }

  // Read joystick 2 values
  int joystick2Y = analogRead(joystick2YPin); // Y-axis value for throttle (0 to 1023)

  // Apply dead zone to Joystick 1 X-axis
  if (joystick1X > 512 - deadZone && joystick1X < 512 + deadZone) {
    joystick1X = 512; // Center the value if within the dead zone
  }

  // Apply dead zone to Joystick 1 Y-axis
  if (joystick1Y > 512 - deadZone && joystick1Y < 512 + deadZone) {
    joystick1Y = 512; // Center the value if within the dead zone
  }

  // Apply dead zone to Joystick 2 Y-axis
  if (joystick2Y > 512 - deadZone && joystick2Y < 512 + deadZone) {
    joystick2Y = 512; // Center the value if within the dead zone
  }

  // Map joystick 1 values to servo angles
  int servo1Angle = map(joystick1X, 0, 1023, minServoAngle, maxServoAngle);  // X-axis normal
  int servo2Angle = map(joystick1X, 0, 1023, maxServoAngle, minServoAngle);  // X-axis opposite
  int servo3Angle = map(joystick1Y, 0, 1023, minServoAngle, maxServoAngle);  // Y-axis same direction
  int servo4Angle = map(joystick1Y, 0, 1023, minServoAngle, maxServoAngle);  // Y-axis same direction

  // Map joystick 2 Y-axis to motor throttle
  int throttle = map(joystick2Y, 0, 1023, minThrottle, maxThrottle); // Map throttle range

  // Write angles to servos
  servo1.write(servo1Angle);
  servo2.write(servo2Angle);
  servo3.write(servo3Angle);
  servo4.write(servo4Angle);

  // Write throttle to ESC
  esc.write(throttle);

  // Debugging output
  Serial.print("Joystick 1 X: ");
  Serial.print(joystick1X);
  Serial.print(" | Servo 1: ");
  Serial.print(servo1Angle);
  Serial.print(" | Servo 2: ");
  Serial.println(servo2Angle);

  Serial.print("Joystick 1 Y: ");
  Serial.print(joystick1Y);
  Serial.print(" | Servo 3: ");
  Serial.print(servo3Angle);
  Serial.print(" | Servo 4: ");
  Serial.println(servo4Angle);

  Serial.print("Joystick 2 Y (Throttle): ");
  Serial.print(joystick2Y);
  Serial.print(" | Throttle: ");
  Serial.println(throttle);

  delay(20); // Add a short delay for smooth motion
}

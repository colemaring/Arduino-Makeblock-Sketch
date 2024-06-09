#include <MeMegaPi.h>
#define motorSpeed 200

// https://forum.makeblock.com/t/programming-ultimate-2-0-gripper/19956

// TODO 
// write crane up and down 
// - since we only have relative position of motor, we can detect mechanical limits of the crane motion by checking the current speed of the motor / the distnace the motor has traveled since last x time. if it is low then you have reached the mechanical limit
// calibrate motors for distance and rotation on carpet


 // left / front track motor is plugged into middle 185 rpm
 // right / back track motor is plugged into left 185 rpm
 // crane motor is plugged into right 86 rpm

// crane
const byte interruptPin1B = 18; // Port1B interrupt pin
const byte NE1B = 31;          // Port1B comparison pin
// track
const byte interruptPin2B = 19; // Port2B interrupt pin
const byte NE2B = 38;          // Port2B comparison pin
// track
const byte interruptPin3B = 3; // Port3B interrupt pin
const byte NE3B = 49;          // Port3B comparison pin
// grabber
const byte interruptPin4B = 2; // Port4B interrupt pin
const byte NE4B = A1;          // Port4B comparison pin

MeMegaPiDCMotor craneMotor(PORT1B);
MeMegaPiDCMotor trackMotor1(PORT2B);
MeMegaPiDCMotor trackMotor2(PORT3B);
MeMegaPiDCMotor grabberMotor(PORT4B);

long count = 0;
long count1 = 0;
long count4 = 0;
boolean isRunning = false;

void setup() {
    pinMode(interruptPin1B, INPUT_PULLUP);
    pinMode(NE1B, INPUT);
    pinMode(interruptPin2B, INPUT_PULLUP);
    pinMode(NE2B, INPUT);
    pinMode(interruptPin3B, INPUT_PULLUP);
    pinMode(NE3B, INPUT);
    pinMode(interruptPin4B, INPUT_PULLUP);
    pinMode(NE4B, INPUT);
    attachInterrupt(digitalPinToInterrupt(interruptPin1B), blink1, RISING);
    attachInterrupt(digitalPinToInterrupt(interruptPin2B), blink2, RISING);
    attachInterrupt(digitalPinToInterrupt(interruptPin3B), blink3, RISING);
    attachInterrupt(digitalPinToInterrupt(interruptPin4B), blink4, RISING);
    Serial.begin(9600);
}

void loop() {
  craneMotor.run(100);
  craneMotor.delay(1000);
  craneMotor.stop();
  Serial.println(count1);
}

void rotate(int degrees, char *direction)
{
  count = 0;
  int motorSpeedLocal = motorSpeed;

  if (strcmp(direction, "counterclockwise") == 0)
  {
    motorSpeedLocal *= -1;
  }

  trackMotor1.run(motorSpeedLocal);
  trackMotor2.run(motorSpeedLocal);

    while (abs((count) / 8) < abs(degrees))
  {
   Serial.println("Rotating " + String(direction) + " " + String(degrees) + " degrees.");
   delay(25);
  }

  trackMotor1.stop();
  trackMotor2.stop();
}

void openGrabber()
{
  count = 0;
  grabberMotor.run(-100);
  delay(4000);
  grabberMotor.stop();
}

void closeGrabber()
{
  count = 0;
  grabberMotor.run(100);
  delay(4000);
  grabberMotor.stop();
}

void move(int distance, int speed, char *direction)
{
  count = 0;
  int motorSpeedLocal = speed;

  if (strcmp(direction, "backwards") == 0)
  {
    motorSpeedLocal *= -1;
  }
  trackMotor1.run(motorSpeedLocal);
  trackMotor2.run(-motorSpeedLocal);

    while (abs((count) / 8) < abs(distance))
  {
   Serial.println("Moving " + String(direction) + " " + String(distance) + " units.");
   delay(25);
  }

  trackMotor1.stop();
  trackMotor2.stop();
}

void blink1() {
    if (digitalRead(NE1B) > 0)
        count1++;
    else
        count1--;
}

void blink2() {
    if (digitalRead(NE2B) > 0)
        count++;
    else
        count--;
}

void blink3() {
    if (digitalRead(NE3B) > 0)
        count++;
    else
        count--;
}

void blink4() {
    if (digitalRead(NE4B) > 0)
        count4++;
    else
        count4--;
}
#include <MeMegaPi.h>
#define craneEndstopSens 1
#define craneMotorSpeed 50

// track
const byte interruptPin1B = 18; // Port1B interrupt pin
const byte NE1B = 31;          // Port1B comparison pin
// track
const byte interruptPin2B = 19; // Port2B interrupt pin
const byte NE2B = 38;          // Port2B comparison pin
// crane
const byte interruptPin3B = 3; // Port3B interrupt pin
const byte NE3B = 49;          // Port3B comparison pin
// grabber
const byte interruptPin4B = 2; // Port4B interrupt pin
const byte NE4B = A1;          // Port4B comparison pin

// counter variables to tell the program the motor's position
long count1 = 0;
long count2 = 0;
long count3 = 0;
long count4 = 0;

// initialize motors
MeMegaPiDCMotor trackMotor2(PORT1B);
MeMegaPiDCMotor trackMotor1(PORT2B);
MeMegaPiDCMotor craneMotor(PORT3B);
MeMegaPiDCMotor grabberMotor(PORT4B);

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

/*
Availible functions for you to use.
    move(int distance, "forwards" or "backwards");
    rotate(int degrees, "clockwise" or "counterclockwise");
    craneDown();
    craneUp();
    openGrabber();
    closeGrabber();
    delay(int milliseconds);
    exit(0);
*/

// THIS IS WHERE YOU WILL WRITE YOUR PROGRAM.
void loop() {

}

void craneDown()
{
  long count3Prev = count3;
  craneMotor.run(-craneMotorSpeed);
  
  while (true)
  {
    delay(100);

    // if the relative position between the last two motor states is less than craneEndstopSens
    if (abs(count3 - count3Prev) < craneEndstopSens && count3 != 0)
    {
      Serial.println("BREAK");
      break;
    }
         
    Serial.println(count3);

    // set count3Prev to the previous motor state
    count3Prev = count3;
  }
  craneMotor.stop();  
}

void craneUp()
{
  long count3Prev = count3;
  craneMotor.run(craneMotorSpeed);
  
  while (true)
  {
    delay(100); 

    // if the relative position between the last two motor states is less than craneEndstopSens
    if (abs(count3 - count3Prev) < craneEndstopSens && count3 != 0)
    {
      Serial.println("BREAK");
      break;
    }
         
    Serial.println(count3);

    // set count3Prev to the previous motor state
    count3Prev = count3;
  }
  craneMotor.stop();  
}

void rotate(int degrees, char *direction)
{
  degrees /= 1.8;
  count2 = 0;
  int motorSpeedLocal = 180;

  if (strcmp(direction, "counterclockwise") == 0)
  {
    motorSpeedLocal *= -1;
  }

  trackMotor1.run(motorSpeedLocal);
  trackMotor2.run(motorSpeedLocal);

  // arbitrary numbers to calibrate the motor's rotation to match the friction of a given surface
  while (abs((count2) / 8) < abs(degrees))
  {
   Serial.println("Rotating " + String(direction) + " " + String(degrees) + " degrees.");
   delay(25);
  }

  trackMotor1.stop();
  trackMotor2.stop();
}

void openGrabber()
{
  count2 = 0;
  grabberMotor.run(-100);
  delay(4000);
  grabberMotor.stop();
}

void closeGrabber()
{
  count2 = 0;
  grabberMotor.run(100);
  delay(4000);
  grabberMotor.stop();
}

void move(int distance, char *direction)
{
  distance *= 2.5;
  count2 = 0;
  int motorSpeedLocal = 150;

  if (strcmp(direction, "backwards") == 0)
  {
    motorSpeedLocal *= -1;
  }
  trackMotor1.run(motorSpeedLocal);
  trackMotor2.run(-motorSpeedLocal);

  // arbitrary numbers to calibrate the motor's rotation to match the friction of a given surface
  while (abs((count2) / 8) < abs(distance))
  {
   Serial.println("Moving " + String(direction) + " " + String(distance) + " units.");
   delay(25);
  }

  trackMotor1.stop();
  trackMotor2.stop();
}

// attach counter variables to the outputs of the comparison pins for each motor (encoding)
void blink1() {
    if (digitalRead(NE1B) > 0)
        count1++;
    else
        count1--;
}

void blink2() {
    if (digitalRead(NE2B) > 0)
        count2++;
    else
        count2--;
}

void blink3() {
    if (digitalRead(NE3B) > 0)
        count3++;
    else
        count3--;
}

void blink4() {
    if (digitalRead(NE4B) > 0)
        count4++;
    else
        count4--;
}

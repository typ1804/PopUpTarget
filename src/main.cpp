#include <Arduino.h>
#include <AccelStepper.h>

const int stepPin = 3;
const int dirPin = 4;

#define motorInterfaceType 1

AccelStepper myStepper(motorInterfaceType, stepPin, dirPin);

void setup()
{
  myStepper.setMaxSpeed(1000);
  myStepper.setAcceleration(200);
  myStepper.setSpeed(200);
  myStepper.moveTo(200);
}

void loop()
{
  if (myStepper.distanceToGo() == 0)
  {
    myStepper.moveTo(-myStepper.currentPosition());
  }

  myStepper.run();
}
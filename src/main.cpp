#include <Arduino.h>
#include <AccelStepper.h>

/** general configuration */

/** motor */
const int stepPin = 3;
const int dirPin = 4;
const int enablePin = 5;
const int ms1Pin = 10;
const int ms2Pin = 11;
const int ms3Pin = 12;
AccelStepper stepper(AccelStepper::DRIVER, stepPin, dirPin);

/** homing */
bool homingComplete = false;
long homingInitial = -1;
const int homeStopPin = 8;

void moveTargetIntoPositionHit() {
//    if (shockDetected) {
//        if (stepper.distanceToGo() == 0) {
//            stepper.moveTo(-stepper.currentPosition());
//        }
//        stepper.runToPosition();
//    }
}

void homing2() {
    while (digitalRead(homeStopPin)) {
        stepper.moveTo(homingInitial);
        homingInitial--;
        stepper.run();
    }

    stepper.stop();
    stepper.setCurrentPosition(0);

    digitalWrite(enablePin, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
}

void setup() {
    stepper.setMaxSpeed(2000);
    stepper.setAcceleration(500);

    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(enablePin, OUTPUT);
    pinMode(ms1Pin, OUTPUT);
    pinMode(ms2Pin, OUTPUT);
    pinMode(ms3Pin, OUTPUT);
    pinMode(homeStopPin, INPUT_PULLUP);

    digitalWrite(ms1Pin, HIGH);
    digitalWrite(ms2Pin, HIGH);
    digitalWrite(ms3Pin, HIGH);

    Serial.begin(115200);
    homing2();
}

void loop() {
//    homing();
}
#include <Arduino.h>
#include <AccelStepper.h>

/** general configuration */

/** pins */
/** motor pins */
const int stepPin = 3;
const int dirPin = 4;
/** shock sensor pins */
const int shockPin = 7;
/** home sensor pins */
const int homeStopPin = 8;

/** variables */
/** shock variables */
bool shockDetected = false;
/**  home sensor variables */
bool homeStopDetected = false;
/**  homing variables */
bool homingComplete = false;
int homingStepsLeft = 5000;
/** motor variables */
AccelStepper stepper(AccelStepper::DRIVER, stepPin, dirPin);

void detectShock() {
    int val = digitalRead(shockPin);

    if (val == HIGH) {
        shockDetected = true;
    } else {
        shockDetected = false;
    }
}

void detectHomeStop() {
    int val = digitalRead(homeStopPin);

    if (val == HIGH) {
        homeStopDetected = true;
    } else {
        homeStopDetected = false;
    }
}

void moveTargetIntoPositionHit() {
//    if (shockDetected) {
//        if (stepper.distanceToGo() == 0) {
//            stepper.moveTo(-stepper.currentPosition());
//        }
//        stepper.runToPosition();
//    }
}

void moveTargetIntoPositionHome() {
    stepper.moveTo(homingStepsLeft);
    stepper.setSpeed(200);

    if (!homingComplete) {
        if (stepper.currentPosition() != stepper.targetPosition()) {
            stepper.run();
            homingStepsLeft -= stepper.currentPosition();
        }

        if (homeStopDetected) {
            stepper.stop();
            stepper.runSpeed();
            stepper.setCurrentPosition(0);
            stepper.moveTo(0);

            homingComplete = true;
            homingStepsLeft = 0;
        }
    }
}

void stopMotorIfStopDetected() {
    if (homeStopDetected) {
        stepper.stop();
        stepper.moveTo(0);
        stepper.runSpeed();
    }
}

void setup() {
    stepper.setMaxSpeed(200);
    stepper.setAcceleration(200);

    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(shockPin, INPUT);
    pinMode(homeStopPin, INPUT);

    Serial.begin(115200);
}

void loop() {
    detectShock();
    detectHomeStop();
    stopMotorIfStopDetected();
    moveTargetIntoPositionHome();
//    moveTargetIntoPositionHit();


    if (homeStopDetected) {
        digitalWrite(LED_BUILTIN, HIGH);
    } else {
        digitalWrite(LED_BUILTIN, LOW);
    }
}
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

/** knock */
const int knockPin = A0;
const int knockThreshold = 100;
int knockReading = 0;
bool knockDetected = false;

/** positions */
const int shootingPosition = 900;
const int hitPosition = 100;
const int resetShootingPositionDelay = 5000;

void detectMoveTargetIntoPositionShoot() {
    if (homingComplete && !knockDetected) {
        if (stepper.currentPosition() != shootingPosition) {
            stepper.runToNewPosition(shootingPosition);
            delay(resetShootingPositionDelay);
        }
    }
}

void detectMoveTargetIntoPositionHit() {
    if (homingComplete && knockDetected) {
        if (stepper.currentPosition() != hitPosition) {
            stepper.runToNewPosition(hitPosition);
            knockDetected = false;
        }
    }
}

void homing() {
    while (digitalRead(homeStopPin)) {
        stepper.moveTo(homingInitial);
        stepper.run();
        homingInitial--;
    }

    stepper.stop();
    stepper.run();
    stepper.setCurrentPosition(0);
    homingComplete = true;
}

void detectKnock() {
    knockReading = analogRead(knockPin);

    if (!knockDetected) {
        if (stepper.currentPosition() > shootingPosition - 10 && stepper.currentPosition() < shootingPosition + 10) {
            if (knockReading >= knockThreshold) {
                knockDetected = true;
            }
        }
    }
}

void setup() {
    Serial.begin(115200);

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

    homing();
}

void loop() {
    detectKnock();
    detectMoveTargetIntoPositionShoot();
    detectMoveTargetIntoPositionHit();
}
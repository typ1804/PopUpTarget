#include <Arduino.h>
#include <AccelStepper.h>

/** general configuration */
const bool debug = true;

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
/** motor variables */
AccelStepper stepper(AccelStepper::DRIVER, stepPin, dirPin);

void log(const String &data) {
    if (debug) {
        Serial.println(data);
    }
}

void detectShock() {
    int val = digitalRead(shockPin);

    if (val == HIGH) {
        shockDetected = true;
        log("shock detected");
    } else {
        shockDetected = false;
        log("no shock detected");
    }
}

void detectHomeStop() {
    int val = digitalRead(homeStopPin);

    if (val == HIGH) {
        homeStopDetected = true;
        log("home stop detected");
    } else {
        homeStopDetected = false;
        log("no home stop detected");
    }
}

void moveTargetIntoPositionHit() {
    if (shockDetected) {
        if (stepper.distanceToGo() == 0) {
            stepper.moveTo(-stepper.currentPosition());
        }
        stepper.runToPosition();

        log("moving target into position: hit");
    }
}

void setup() {
    stepper.setMaxSpeed(2000);
    stepper.setAcceleration(200);
    stepper.setSpeed(1000);
    stepper.moveTo(500);

    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(shockPin, INPUT);
    pinMode(homeStopPin, INPUT);

    if (debug) {
        Serial.begin(115200);
        log("setup");
    }
}

void loop() {
//    detectShock();
    detectHomeStop();
//    moveTargetIntoPositionHit();

    if (homeStopDetected) {
        digitalWrite(LED_BUILTIN, HIGH);
    } else {
        digitalWrite(LED_BUILTIN, LOW);
    }
}
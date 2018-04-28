/* Shade control
 * (C) 2016 by Michael Stuermer <ms@mallorn.de>
 * 
 * Hardware: Arduino Nano
 */ 
#include <Wire.h>
#include <SparkFunTSL2561.h>
#include <Stepper.h>
#include <Encoder.h>

#define PIN_ENCODER_SW 4
#define PIN_ENCODER_A 2
#define PIN_ENCODER_B 3

#define MODE_ORIENTATE 1
#define MODE_MANUAL    2
#define MODE_AUTOMATIC 3

const int stepsPerRevolution = 2048;

#define LUMINOSITY_LOW  80
#define LUMINOSITY_HIGH 150

#define POSITION_MIN (-2048 * 3)
#define POSITION_MAX (2048 * 3)

#define POSITION_BRIGHT 0
#define POSITION_DARK POSITION_MAX

int mode;

void setup() {
  Serial.begin(115200);
  Serial.println(F("shade_control"));
  mode = MODE_ORIENTATE;
  setupSensor();
  setupStepper();
  setupEncoder();
}

long loop_time = 0;
void loop() {
  loop_time = millis();

  loopSensor();
  loopEncoder();
  loopStepper();
}

#include <limits.h>
Encoder encoder(PIN_ENCODER_A, PIN_ENCODER_B);

void setupEncoder() {
  pinMode(PIN_ENCODER_SW, INPUT_PULLUP);
}

long encoderPos = 0;

long switchDownTime = -1;
boolean oldSwitchState = false;

long nextEncoderTime = 0;
void loopEncoder() {
  if (loop_time >= nextEncoderTime) {
    nextEncoderTime = loop_time + 1;
  
    long newEncoderPos = encoder.read() / 4; // no half skips
    long delta = newEncoderPos - encoderPos;
    if (delta != 0) {
      encoderPos = newEncoderPos;
  
      encoderMoved(delta);
    }
  
    boolean newSwitchState = !digitalRead(PIN_ENCODER_SW);
    if (newSwitchState) {
      if (!oldSwitchState) {
        Serial.println("Down");
        switchDownTime = loop_time;
      }
    } else {
      if (switchDownTime >= 0 && loop_time - switchDownTime > 30) {
        Serial.println("Press");
        switchDownTime = -1;
  
        encoderSwitchPress();
      }
    }
    oldSwitchState = newSwitchState;
  }
}

void encoderMoved(long delta) {
  switch (mode) {
    case MODE_ORIENTATE:
      setStepperPosition(getStepperPosition() + delta * (stepsPerRevolution / 8), false);
      break;
    case MODE_MANUAL:
      setStepperPosition(getStepperPosition() + delta * (stepsPerRevolution / 8), true);
      break;
    case MODE_AUTOMATIC:
      setStepperPosition(getStepperPosition() + delta * (stepsPerRevolution / 8), true);
      break;
  }
}

void encoderSwitchPress() {
  switch (mode) {
    case MODE_ORIENTATE:
      resetStepperPosition();
      mode = MODE_AUTOMATIC;
      Serial.println("set mode automatic");
      stepperSignal(30);
      delay(100);
      stepperSignal(90);
      delay(100);
      break;
    case MODE_MANUAL:
      mode = MODE_AUTOMATIC;
      Serial.println("set mode automatic");
      stepperSignal(30);
      delay(100);
      stepperSignal(90);
      delay(100);
      break;
    case MODE_AUTOMATIC:
      mode = MODE_MANUAL;
      Serial.println("set mode manual");
      stepperSignal(90);
      delay(100);
      stepperSignal(90);
      delay(100);
      break;
  }
}


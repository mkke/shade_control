// for the 28BYJ-48, pin 2+3 have to be swapped

Stepper stepper(stepsPerRevolution, 5, 7, 6, 8);

void disableStepperPower() {
  digitalWrite(5, LOW);
  digitalWrite(7, LOW);
  digitalWrite(6, LOW);
  digitalWrite(8, LOW);
}

void setupStepper() {
  stepper.setSpeed(12);
}

long position = 0;

long targetPosition = 0;
long getStepperPosition() {
  return targetPosition;
}

void setStepperPosition(long newPosition, boolean obeyStops) {
  if (obeyStops) {
    targetPosition = constrain(newPosition, POSITION_MIN, POSITION_MAX);
  } else {
    targetPosition = newPosition;
  }
}

void resetStepperPosition() {
  position = 0;
  targetPosition = 0;
  disableStepperPower();
}

void rotate(int steps) {
  //Serial.print("rotating stepper by ");
  //Serial.println(steps);
  stepper.step(steps);
}

void stepperSignal(int count) {
  for (int i = 0; i < count; i++) {
    stepper.step(1);
    stepper.step(-1);
  }
}

long nextStepperTime = 0;
void loopStepper() {
  int delta = targetPosition - position;
  if (delta != 0) {
    Serial.print("targetPosition = ");
    Serial.println(targetPosition);

    rotate(delta);
    position += delta;
    if (position == 0) {
      disableStepperPower();
    }
  }
}


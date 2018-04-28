SFE_TSL2561 tsl;

unsigned char ID;
boolean sensorFound;
unsigned int ms;  // Integration ("shutter") time in milliseconds
long nextSensorTime;
void setupSensor() {
  if (tsl.begin() && tsl.getID(ID)) {
    sensorFound = true;
    tsl.setTiming(0, 2 /* 402ms */, ms);
    tsl.setPowerUp();
    nextSensorTime = millis() + ms;
  } else {
    Serial.println("Sensor not found");
  }
}

void loopSensor() {
  if (loop_time >= nextSensorTime) {
    nextSensorTime = loop_time + ms;

    if (sensorFound) {
      unsigned int data0, data1;
      if (tsl.getData(data0,data1)) {
        double lux;
        boolean notSaturated;
        notSaturated = tsl.getLux(0,ms,data0,data1,lux);
        Serial.println(lux);

        if (mode == MODE_AUTOMATIC) {
          if (lux < LUMINOSITY_LOW && getStepperPosition() > POSITION_BRIGHT) {
            setStepperPosition(getStepperPosition() + (POSITION_BRIGHT - POSITION_DARK) / 64, true);
          } else if (lux > LUMINOSITY_HIGH && getStepperPosition() < POSITION_DARK) {
            setStepperPosition(getStepperPosition() + (POSITION_DARK - POSITION_BRIGHT) / 64, true);            
          }
        }
      }
    }
  }
}


#ifndef SensorColor_H
#define SensorColor_H

#include "Arduino.h"
#include "Adafruit_TCS34725.h"


class SensorColor {
public:
  
  SensorColor(Adafruit_TCS34725 tcs);
  ~SensorColor();
  void detectColor();
  void calculateColor(Adafruit_TCS34725 tcs);

private:

  float current_r;
  float current_g;
  float current_b;
  float detected_r;
  float detected_g;
  float detected_b;
};

#endif

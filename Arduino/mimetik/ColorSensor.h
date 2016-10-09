#ifndef ColorSensor_H
#define ColorSensor_H

#include "Arduino.h"
#include "Adafruit_TCS34725.h"
#include "States.h"

class ColorSensor {
public:
    // color variables
   uint16_t clr, red, green, blue;
   uint32_t sum;
  ColorSensor(Adafruit_TCS34725 tcs);
  ~ColorSensor();
  int detectColor();
  void calibrate(Adafruit_TCS34725 tcs);
  void calculateColor(Adafruit_TCS34725 tcs, States *stateObj);
  int getMax(float r,float g,float b);
  int getMin(float r,float g,float b);
  float getMaxValue(float r, float g, float b);
  void printResults();
  bool firstDetection = true;
  
private:
  
  float current_r;
  float current_g;
  float current_b;
  float c_red = 0;
  float c_green = 0;
  float c_blue = 0;
  float detected_r;
  float detected_g;
  float detected_b;
  int max_rgb=-1;

  
  float timer = 0;
};

#endif

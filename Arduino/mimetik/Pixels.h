#ifndef Pixels_H
#define Pixels_H

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>



class Pixels {
public:
  Pixels();
  ~Pixels();
  
  #define PIN            3
  #define NUMPIXELS      6

  //Adafruit_NeoPixel pixels;

  
  void randomColor(Adafruit_NeoPixel &_pixels);
  
  void RGBColor(Adafruit_NeoPixel &_pixels, int _red, int _green, int _blue);
  
  void ColorShift(Adafruit_NeoPixel &_pixels, String _kColor, float _accx);
  
  uint32_t Wheel(Adafruit_NeoPixel &_pixels,byte WheelPos);

  
private:

};

#endif

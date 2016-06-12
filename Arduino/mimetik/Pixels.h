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
  
  void RGBColor(Adafruit_NeoPixel &_pixels, String _color);

  
private:

};

#endif

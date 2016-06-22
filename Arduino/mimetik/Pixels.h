#ifndef Pixels_H
#define Pixels_H

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>
#include "Vector3.h"


class Pixels {
public:
  Pixels();
  ~Pixels();

  #define PIN            3
  #define NUMPIXELS      40

  //Adafruit_NeoPixel pixels;


  unsigned long Interval;   // milliseconds between updates
  unsigned long lastUpdate; // last update of position
  uint16_t TotalSteps;  // total number of steps in the pattern
  float Index;  // current step within the pattern
    
  void (*OnComplete)();  // Callback on completion of pattern


  void randomColor(Adafruit_NeoPixel &_pixels);

  void RGBColor(Adafruit_NeoPixel &_pixels, int _red, int _green, int _blue);

  void transitionPixels(Adafruit_NeoPixel &_pixels, String _newColor, String _oldColor);

  void ColorShift(Adafruit_NeoPixel &_pixels, String _kColor, float _accx);
  void Increment();

  Vector3 returnColorValues(Adafruit_NeoPixel &_pixels, String _color);

  //void ColorSwirl(Adafruit_NeoPixel &_pixels, String _kColor, float _accx);

  uint32_t Wheel(Adafruit_NeoPixel &_pixels,byte WheelPos);


private:

};

#endif

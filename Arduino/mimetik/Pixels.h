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

    int Index;  // current step within the pattern

    void randomColor(Adafruit_NeoPixel &_pixels);
    void RGBColor(Adafruit_NeoPixel &_pixels, int _red, int _green, int _blue);

    // TRANSITION MODE
    void transitionPixels(Adafruit_NeoPixel &_pixels, char _newColor, char _oldColor, float timeTrans);
    void wupTransitionPixels(Adafruit_NeoPixel &_pixels, char _newColor, char _oldColor, float timeTrans);

    // STATE MODE
    void ColorShift(Adafruit_NeoPixel &_pixels, char _kColor, float _accx, float _accy, float _accz, float _time);

    uint32_t Wheel(Adafruit_NeoPixel &_pixels, byte WheelPos);

    float initTimeTransition;
    float finishTimeTransition;
    uint32_t r;
    uint32_t g;
    uint32_t b;
    uint32_t color_mix;
    //int iteration[40] = {0, 14, 20, 34, 15, 16, 17, 18, 19, 35, 36, 37, 38, 39, 1, 13, 21, 33, 2, 12, 22, 32, 3, 11, 23, 31, 4, 10, 24, 30, 5, 6, 7, 8, 9, 25, 26, 27, 28, 29};

  // Color tint
  float r_tint;
  float g_tint;
  float b_tint;

  private:

};

#endif

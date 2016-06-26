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
    // transition animation repetitions
#define repetitions 6.0
// period and frequency
float T;
float freq;

float brightness;

    //int Index;  // current step within the pattern

    void randomColor(Adafruit_NeoPixel &_pixels);
    void RGBColor(Adafruit_NeoPixel &_pixels, uint8_t _red, uint8_t _green, uint8_t _blue);

    // SLEEP MODE
    void sleepPixels(Adafruit_NeoPixel &_pixels);

    // TRANSITION MODE
    void transitionPixels(Adafruit_NeoPixel &_pixels, char _newColor, char _oldColor, float timeTrans, float accx, float accy, float accz, const long _interval);

    // STATE MODE
    void ColorShift(Adafruit_NeoPixel &_pixels, char _kColor, float _accx, float _accy, float _accz, float _time);

    uint32_t Wheel(Adafruit_NeoPixel &_pixels, byte WheelPos);
    float mapfloat(float x, float in_min, float in_max, float out_min, float out_max);
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
    float r_tint2;
    float g_tint2;
    float b_tint2;

  private:

};

#endif

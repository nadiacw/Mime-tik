#include "Pixels.h"
#include "Arduino.h"

//#include <Adafruit_NeoPixel.h>


Pixels::Pixels()
{
  Index = 0;
  initTimeTransition = 0;
  finishTimeTransition = 0;
}

Pixels::~Pixels()
{

}

void Pixels::randomColor(Adafruit_NeoPixel &_pixels)
{
  Serial.println("in rand");
  for (int i = 0; i < NUMPIXELS; i++) {
    _pixels.setPixelColor(i, _pixels.Color(random(0, 255), random(0, 255), random(0, 255)));
    _pixels.show();
  }
}

void Pixels::RGBColor(Adafruit_NeoPixel &_pixels, int _red, int _green, int _blue)
{
  for (int i = 0; i < NUMPIXELS; i++) {
    _pixels.setPixelColor(i, _pixels.Color(_red, _green, _blue));
    _pixels.show();
  }
}

void Pixels::transitionPixels(Adafruit_NeoPixel &_pixels, char _newColor, char _oldColor, float timeTrans, float _accx, float _accy, float _accz, const long _interval) {
  T = _interval / (_interval * repetitions);
  freq = 1.0 / T;
  float brightness = mapfloat(sin((timeTrans + 0.25 * T) * 3.14 * 2 * freq ), -1, 1, 50, 255);
  _pixels.setBrightness(brightness);

  Vector3 color_new = Vector3(0, 0, 0);
  Vector3 color_old = Vector3(0, 0, 0);
  //color_new = returnColorValues(_newColor);
  //color_old = returnColorValues(_oldColor);

  if (_newColor == 'r') {
    color_new = Vector3(255, 0, 0);
  }
  else if (_newColor == 'g') {
    color_new = Vector3(0, 255, 0);
  }
  else if (_newColor == 'b') {
    color_new = Vector3(0, 0, 255);
  }
  else {
    Serial.println("MAAAAAAAL");
  }


  if (_oldColor == 'r') {
    color_old = Vector3(255, 0, 0);
  }
  else if (_oldColor == 'g') {
    color_old = Vector3(0, 255, 0);
  }
  else if (_oldColor == 'b') {
    color_old = Vector3(0, 0, 255);
  }
  else {
    Serial.println("MU MAAAL");
  }

  if (_newColor == 'r') {
    //get tint
    r_tint = map(_accx, 0, 10, 220, 255); g_tint = map(_accy, 0, 10, 0, 30); b_tint = map(_accz, 0, 10, 0, 30);
  }
  else if (_newColor == 'g') {
    r_tint = map(_accy, 0, 10, 0, 50); g_tint = map(_accx, 0, 10, 220, 255); b_tint = map(_accz, 0, 10, 50, 0);
  }
  else if (_newColor == 'b') {
    r_tint = map(_accz, 0, 10, 0, 50); g_tint = map(_accy, 0, 10, 0, 50); b_tint = map(_accx, 0, 10, 220, 255);
  }

  if (_oldColor == 'r') {
    //get tint
    r_tint2 = map(_accx, 0, 10, 220, 255); g_tint2 = map(_accy, 0, 10, 0, 30); b_tint2 = map(_accz, 0, 10, 0, 30);
  }
  else if (_oldColor == 'g') {
    r_tint2 = map(_accy, 0, 10, 0, 50); g_tint2 = map(_accx, 0, 10, 220, 255); b_tint2 = map(_accz, 0, 10, 50, 0);
  }
  else if (_oldColor == 'b') {
    r_tint2 = map(_accz, 0, 10, 0, 50); g_tint2 = map(_accy, 0, 10, 0, 50); b_tint2 = map(_accx, 0, 10, 220, 255);
  }

  if (timeTrans <= 1) {
    r = color_old.x * (1 - timeTrans) + color_new.x * timeTrans + (r_tint * (timeTrans) + r_tint2 * (1 - timeTrans));
    g = color_old.y * (1 - timeTrans) + color_new.y * timeTrans + (g_tint * (timeTrans) + g_tint2 * (1 - timeTrans));
    b = color_old.z * (1 - timeTrans) + color_new.z * timeTrans + (b_tint * (timeTrans) + b_tint2 * (1 - timeTrans));
    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;

    color_mix = _pixels.Color(r, g, b);

    for (int i = 0; i < NUMPIXELS; i++ ) {
      _pixels.setPixelColor(i, color_mix);
      _pixels.show();
    }
  }
  //color_new.x = 0; color_new.y = 0; color_new.z = 0;
  //color_old.x = 0; color_old.y = 0; color_old.z = 0;
}

void Pixels::wupTransitionPixels(Adafruit_NeoPixel &_pixels, char _newColor, char _oldColor, float timeTrans) {
  //  Vector3 color_new = returnColorValues(_newColor);
  //  Vector3 color_old = returnColorValues(_oldColor);
  //
  //  for (float i = 0; i < NUMPIXELS; i++) {
  //    uint32_t r = color_old.x * (1 - i / (float)NUMPIXELS) + color_new.x * i / NUMPIXELS;
  //    uint32_t g = color_old.y * (1 - i / (float)NUMPIXELS) + color_new.y * i / NUMPIXELS;
  //    uint32_t b = color_old.z * (1 - i / (float)NUMPIXELS) + color_new.z * i / NUMPIXELS;
  //
  //    uint32_t color_mix = _pixels.Color(r, g, b);
  //    Serial.println(i + (int)(timeTrans * 40) - 40);
  //
  //    if (i + (int)timeTrans * NUMPIXELS - NUMPIXELS < 0)
  //    {
  //      continue;
  //    }
  //    else if (i + (int)timeTrans * NUMPIXELS - NUMPIXELS >= 40)
  //    {
  //      continue;
  //    }
  //    _pixels.setPixelColor(i + (int)timeTrans * NUMPIXELS - NUMPIXELS, color_mix);
  //  }
  //  _pixels.show();
  //  Serial.println("hello");

}

void Pixels::ColorShift(Adafruit_NeoPixel &_pixels, char _kColor, float _accx, float _accy, float _accz, float _time) {

  Serial.print(_accx);
  Serial.print(" ");
  Serial.print(_accy);
  Serial.print(" ");
  Serial.print(_accz);


  /*Serial.print(" / ");
    Serial.print(r_tint);
    Serial.print(" ");
    Serial.print(g_tint);
    Serial.print(" ");
    Serial.print(b_tint);*/


  if (_kColor == 'r') {
    //get tint
    r_tint = map(_accx, 0, 10, 220, 255);
    g_tint = map(_accy, 0, 10, 0, 30);
    b_tint = map(_accz, 0, 10, 0, 30);
    for (int i = 0; i < NUMPIXELS; i++ ) {
      _pixels.setPixelColor(i, _pixels.Color(r_tint, g_tint, b_tint));
      _pixels.show();
    }
  }

  else if (_kColor == 'g') {
    r_tint = map(_accy, 0, 10, 0, 50);
    g_tint = map(_accx, 0, 10, 220, 255);
    b_tint = map(_accz, 0, 10, 50, 0);
    for (int i = 0; i < NUMPIXELS; i++ ) {
      _pixels.setPixelColor(i, _pixels.Color(r_tint, g_tint, b_tint));
      _pixels.show();
    }
  }

  else if (_kColor == 'b') {
    r_tint = map(_accz, 0, 10, 0, 50);
    g_tint = map(_accy, 0, 10, 0, 50);
    b_tint = map(_accx, 0, 10, 220, 255);
    for (int i = 0; i < NUMPIXELS; i++ ) {
      _pixels.setPixelColor(i, _pixels.Color(r_tint, g_tint, b_tint));
      _pixels.show();
    }
  }
}

uint32_t Pixels::Wheel(Adafruit_NeoPixel &_pixels, byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return _pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return _pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
    WheelPos -= 170;
    return _pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

float Pixels::mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

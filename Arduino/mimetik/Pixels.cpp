#include "Pixels.h"
#include "Arduino.h"

//#include <Adafruit_NeoPixel.h>


Pixels::Pixels()
{
  Index = 0;
  Interval = 1;
  TotalSteps = 255;
}

Pixels::~Pixels()
{

}

void Pixels::randomColor(Adafruit_NeoPixel &_pixels)
{
  Serial.println("in rand");
  int red = random(0, 255);
  int green = random(0, 255);
  int blue = random(0, 255);
  for (int i = 0; i < NUMPIXELS; i++) {
    _pixels.setPixelColor(i, _pixels.Color(red, green, blue));
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

void Pixels::transitionPixels(Adafruit_NeoPixel &_pixels, String _newColor, String _oldColor)
{
  Vector3 color_new = returnColorValues(_pixels, _newColor);
  Vector3 color_old = returnColorValues(_pixels, _oldColor);
  while (Index <= 1) {
  
    
    uint32_t r = color_old.x*(1-Index) + color_new.x*Index;
    uint32_t g = color_old.y*(1-Index) + color_new.y*Index;
    uint32_t b = color_old.z*(1-Index) + color_new.z*Index;
  
    uint32_t color_mix = _pixels.Color(r,g,b);
    
    for (int i = 0; i < NUMPIXELS; i++)
    {
        _pixels.setPixelColor(i, color_mix);
        _pixels.show();
     }
     
     Index = Index + 0.008;
  
  }
  Index = 0;

}

Vector3 Pixels::returnColorValues(Adafruit_NeoPixel &_pixels, String _color) {
  if (_color == "red") {
    return Vector3(255, 0, 0);
  }
  else if  (_color == "green") {
    return Vector3(0, 255, 0);
  }
  else if  (_color == "blue") {
   return Vector3(0, 0, 255);
  }
}

void Pixels::ColorShift(Adafruit_NeoPixel &_pixels, String _kColor, float _accx) {

  //Serial.print(_accx);
  if (_kColor == "blue") {
    for (int i = 0; i < NUMPIXELS; i++)
    {
      //_pixels.setPixelColor(i, Wheel(_pixels, ((i * 256 / NUMPIXELS) + Index) & 255));
    }
    _pixels.show();
    Index++;
  }
  else if (_kColor == "red") {
    for (int i = 0; i < NUMPIXELS; i++) {
      _pixels.setPixelColor(i, _pixels.Color(255, 0, 0));
      _pixels.show();
    }
  }
  else if (_kColor == "green") {
    for (int i = 0; i < NUMPIXELS; i++) {
      _pixels.setPixelColor(i, _pixels.Color(0, 255, 0));
      _pixels.show();
    }
  }
}

void Pixels::Increment()
{
  Index++;
}



// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
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

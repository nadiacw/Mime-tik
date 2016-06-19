#include "Pixels.h"
#include "Arduino.h"
//#include <Adafruit_NeoPixel.h>


Pixels::Pixels()
{
  previousMillis = 0;
  //interval = 1000;
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

void Pixels::ColorShift(Adafruit_NeoPixel &_pixels, String _kColor, float _accx) {

  //Serial.print(_accx);
  if (_kColor == "blue") {
    uint16_t i = 0;
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

        for (i = 0; i < _pixels.numPixels(); i++) {
          _pixels.setPixelColor(i, Wheel(_pixels, ((i * 256 / _pixels.numPixels()) + pixItj) & 255));
        }
        _pixels.show();

      if(pixItj < 256 * 5)
      {
        pixItj++;
      }
      else
      {
        pixItj = 0;
      }
    }
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

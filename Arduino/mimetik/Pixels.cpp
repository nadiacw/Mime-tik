#include "Pixels.h"
#include "Arduino.h"
//#include <Adafruit_NeoPixel.h>


Pixels::Pixels()
{

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

void Pixels::RGBColor(Adafruit_NeoPixel &_pixels, String _color)
{
  int red = 0;
  int green = 0;
  int blue = 0;
  if(_color = "red"){
    red = 255;
  }
  else if(_color = "green"){
    green = 255;
  }
  else if(_color = "blue"){
    blue = 255;
  }
  
  for (int i = 0; i < NUMPIXELS; i++) {
    _pixels.setPixelColor(i, _pixels.Color(red, green, blue));
    _pixels.show();
  }
  
}

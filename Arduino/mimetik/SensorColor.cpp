#include "SensorColor.h"
#include "Arduino.h"

SensorColor::SensorColor(Adafruit_TCS34725 tcs)
{
  /**************** COLOR SENSOR CONGIF ********************/
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1); // halt!
  }
}

SensorColor::~SensorColor()
{
}

void SensorColor::calculateColor(Adafruit_TCS34725 tcs)
{
  // color variables
   uint16_t clear, red, green, blue;
   tcs.setInterrupt(false);      // turn on LED
   delay(60);  // takes 50ms to read
   tcs.getRawData(&red, &green, &blue, &clear);
   tcs.setInterrupt(true);  // turn off LED

   // Figure out some basic hex code for visualization
   uint32_t sum = clear;
   current_r = red; current_r /= sum;
   current_g = green; current_g /= sum;
   current_b = blue; current_b /= sum;
   current_r *= 256; current_g *= 256; current_b *= 256;


   Serial.print("C:\t"); Serial.print(clear);
   Serial.print("\tR:\t"); Serial.print(red);
   Serial.print("\tG:\t"); Serial.print(green);
   Serial.print("\tB:\t"); Serial.print(blue);
   Serial.println("");
}
void SensorColor::detectColor()
{

}

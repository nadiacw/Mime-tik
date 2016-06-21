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


// Calculate color from the color sensor
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
   
}


// detect which color is to display the states
int SensorColor::detectColor()
{
  int temp_max_rgb;
  int totalSum = (current_r + current_g + current_b);

  if(abs(current_r / totalSum - 0.33)*100 > 10)
  {
    temp_max_rgb = getMax(current_r,current_g,current_b);
  }
  else
  {
    return max_rgb;
  }
  
  if(max_rgb != temp_max_rgb && firstDetection)
  {
    firstDetection = false;
    timer = millis();
    Serial.print("First detection done");
  }
  else if(max_rgb != temp_max_rgb && !firstDetection)
  {
    if(millis() - timer > 1000)
    {
      max_rgb = temp_max_rgb;
      firstDetection = true;
      timer = 0;
      Serial.print("Detection done: ");
      Serial.print(max_rgb);
    }
  }
  else
  {
    //Serial.print("Colors are equal");
    firstDetection = true;
    timer = 0;
  }
  Serial.println("");
  
  return max_rgb;
}

int SensorColor::getMax(float r,float g,float b)
{
    if(r > g)
    {
      if(r > b)
      {
        return 0;
      }
      else
      {
        return 2;
      }
    } 
    else if(g > b)
    {
        return 1;
    } 
    else
    {
      return 2;
    }
 }

 
// print all the results
 void SensorColor::printResults() {
  
   
   Serial.print("R:\t"); Serial.print(current_r);
   Serial.print("\tG: "); Serial.print(current_g);
   Serial.print("\tB: "); Serial.print(current_b);
   Serial.print("\tMax value: "); Serial.print(max_rgb);
   Serial.print(" --> "); 
   
 }

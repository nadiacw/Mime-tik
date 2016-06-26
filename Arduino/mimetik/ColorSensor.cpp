#include "ColorSensor.h"
#include "Arduino.h"

ColorSensor::ColorSensor(Adafruit_TCS34725 tcs)
{
  /**************** COLOR SENSOR CONGIF ********************/
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1); // halt!
  }
}

ColorSensor::~ColorSensor()
{
}


void ColorSensor::calibrate(Adafruit_TCS34725 tcs) {
  tcs.setInterrupt(false);      // turn on LED
  delay(60);  // takes 50ms to read
  tcs.getRawData(&red, &green, &blue, &clr);
  tcs.setInterrupt(true);  // turn off LED
  sum = clr;
  // Figure out some basic hex code for visualization
  current_r = red; current_r /= sum;
  current_g = green; current_g /= sum;
  current_b = blue; current_b /= sum;
  current_r *= 256; current_g *= 256; current_b *= 256;

  uint8_t minColor = getMin(c_red, c_green, c_blue);

  Serial.println(minColor);

  switch (minColor) {
    case 0:
      c_red = current_r - current_r;
      c_green = current_g - current_r;
      c_blue = current_b - current_r;
      break;
    case 1:
      c_red = current_r - current_g;
      c_green = current_g - current_g;
      c_blue = current_b - current_g;
      break;
    case 2:
      c_red = current_r - current_b;
      c_green = current_g - current_b;
      c_blue = current_b - current_b;
      break;
  }

  
  Serial.println(c_red); Serial.println(c_green); Serial.println(c_blue);

}

// Calculate color from the color sensor
void ColorSensor::calculateColor(Adafruit_TCS34725 tcs)
{
  tcs.setInterrupt(false);      // turn on LED
  delay(60);  // takes 50ms to read
  tcs.getRawData(&red, &green, &blue, &clr);
  tcs.setInterrupt(true);  // turn off LED
  sum = clr;
  // Figure out some basic hex code for visualization
  current_r = red; current_r /= sum;
  current_g = green; current_g /= sum;
  current_b = blue; current_b /= sum;
  current_r *= 256; current_g *= 256; current_b *= 256;

  current_r = abs(current_r - c_red);
  current_g = abs(current_g - c_green);
  current_b = abs(current_b - c_blue);

}


// detect which color is to display the states
int ColorSensor::detectColor()
{
  int temp_max_rgb;
  int totalSum = (current_r + current_g + current_b);
  
Serial.println(abs(current_g / totalSum - 0.33)*100);

  if (abs(current_g / totalSum - 0.33) * 100 > 15)
  {
    temp_max_rgb = getMax(current_r, current_g, current_b);
  }
  else
  {
    return max_rgb;
  }

  if (max_rgb != temp_max_rgb && firstDetection)
  {
    firstDetection = false;
    timer = millis();
    Serial.print("First detection done");
  }
  else if (max_rgb != temp_max_rgb && !firstDetection)
  {
    if (millis() - timer > 2000)
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

int ColorSensor::getMax(float r, float g, float b)
{
  if (r > g)
  {
    if (r > b)
    {
      return 0;
    }
    else
    {
      return 2;
    }
  }
  else if (g > b)
  {
    return 1;
  }
  else
  {
    return 2;
  }
}

int ColorSensor::getMin(float r, float g, float b)
{
  if (r < g)
  {
    if (r < b)
    {
      return 0;
    }
    else
    {
      return 2;
    }
  }
  else if (g < b)
  {
    return 1;
  }
  else
  {
    return 2;
  }
}


// print all the results
void ColorSensor::printResults() {


  Serial.print("R:\t"); Serial.print(current_r);
  Serial.print("\tG: "); Serial.print(current_g);
  Serial.print("\tB: "); Serial.print(current_b);
  Serial.print("\tMax value: "); Serial.print(max_rgb);
  Serial.print(" --> ");

}

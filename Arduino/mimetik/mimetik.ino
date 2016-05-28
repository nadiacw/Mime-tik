//////////////////////////////////////////////////////
// Bluetooth Communication and state implementation //
//////////////////////////////////////////////////////

#include <SoftwareSerial.h>
#include <Wire.h>
#include "Vector3.h"

int x, y, z;
float xm, ym, zm, temp;
int xmin, ymin, zmin, xmax, ymax, zmax;

bool foundNewPossibleFace = false, firstEntry = false;
Vector3 *possibleFace;

Vector3 *currentAcc;
Vector3 *currentFace;

#define LED_PIN 3

const int xpin = A3;                  // x-axis of the accelerometer
const int ypin = A2;                  // y-axis
const int zpin = A1;                  // z-axis (only on 3-axis models)


void setup() {
  //-------------ACC CONFIG ----------------//
  analogReference(EXTERNAL);  // sets the serial port to 9600
  x = analogRead(0);
  y = analogRead(1);
  z = analogRead(2);
  currentAcc = new  Vector3(x,y,z);
  currentFace = new Vector3(1, 1, 0);
}

void loop() {

  x = analogRead(0);       // read analog input pin 0
  delay(1);
  y = analogRead(1);       // read analog input pin 1
  delay(1);
  z = analogRead(2);       // read analog input pin 2
  delay(1);

  Serial.begin(9600);

  //zero_G is the reading we expect from the sensor when it detects
  //no acceleration.  Subtract this value from the sensor reading to
  //get a shifted sensor reading.
  float zero_G = 512.0;

  //scale is the number of units we expect the sensor reading to
  //change when the acceleration along an axis changes by 1G.
  //Divide the shifted sensor reading by scale to get acceleration in Gs.
  float scale = 102.3;

  //continuar...

  int xtemp = constrain(round(((float)x - zero_G) / scale * 127 + 127), 0, 255);
  int ytemp = constrain(round(((float)y - zero_G) / scale * 127 + 127), 0, 255);
  int ztemp = constrain(round(((float)z - zero_G) / scale * 127 + 127), 0, 255);

  currentAcc->x = mapf(xtemp, 0, 255, -1, 1);
  currentAcc->y = mapf(ytemp, 0, 255, -1, 1);
  currentAcc->z = mapf(ztemp, 0, 255, -1, 1);

  detectFace(currentAcc);

  //currentAcc->printValues();

  delay(500);
}

float mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void detectFace(Vector3 *currentAcc)
{
  if(abs(currentAcc->x - currentFace->x) > 0.3 || abs(currentAcc->y - currentFace->y) > 0.3 || abs(currentAcc->y - currentFace->y) > 0.3 ) 
  {
    if(firstEntry)
    {
      foundNewPossibleFace = true;
      possibleFace = currentAcc;
      temp = millis();
      firstEntry = false;
      Serial.print("Has detected possible face: ");
    }
    else
    {
      if(foundNewPossibleFace)
      {
        if (millis() - temp >= 1000) {
          currentFace = possibleFace;
          firstEntry = true;
          Serial.println("Has updated face");
        }
      }
    }
  }
  else
  {
    foundNewPossibleFace = false; 
    Serial.println("Is not moving");
  }  
}


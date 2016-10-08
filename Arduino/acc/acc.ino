#include "Vector3.h"

int x, y, z;
float xm, ym, zm, temp;
int xmin, ymin, zmin, xmax, ymax, zmax, xtemp, ytemp, ztemp;
const int xpin = A3;                  // x-axis of the accelerometer
const int ypin = A2;                  // y-axis
const int zpin = A1;                  // z-axis (only on 3-axis models)
#define zero_G 512.0
#define scale 102.3


bool foundNewPossibleFace = false, firstEntry = false;
Vector3 *possibleFace;
bool isMoving = false;
bool hasMoved = false;

Vector3 *currentAcc;
Vector3 *currentFace;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  analogReference(EXTERNAL);
  x = analogRead(0);
  y = analogRead(1);
  z = analogRead(2);
  currentAcc = new  Vector3(x, y, z);
  currentFace = new Vector3(1, 1, 0);
}

void loop() {
  x = analogRead(0); delay(1); y = analogRead(1); delay(1); z = analogRead(2); delay(1);

  xtemp = constrain(round(((float)x - zero_G) / scale * 127 + 127), 0, 255);
  ytemp = constrain(round(((float)y - zero_G) / scale * 127 + 127), 0, 255);
  ztemp = constrain(round(((float)z - zero_G) / scale * 127 + 127), 0, 255);

  currentAcc->x = mapf(xtemp, 0, 255, 0, 10);
  currentAcc->y = mapf(ytemp, 0, 255, 0, 10);
  currentAcc->z = mapf(ztemp, 0, 255, 0, 10);

  detectFace(currentAcc);

  currentAcc->printValues();

}

float mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void detectFace(Vector3 * currentAcc)
{
  if (abs(currentAcc->x - currentFace->x) > 3 || abs(currentAcc->y - currentFace->y) > 3 || abs(currentAcc->y - currentFace->y) > 3 )
  {
    isMoving = false;
    if (firstEntry)
    {
      foundNewPossibleFace = true;
      possibleFace = currentAcc;
      temp = millis();
      firstEntry = false;
      //Serial.print("Has detected possible face: ");
    }
    else
    {
      if (foundNewPossibleFace)
      {
        if (millis() - temp >= 1000) {
          currentFace = possibleFace;
          firstEntry = true;
          //Serial.println("Has updated face");
        }
      }
    }
  }
  else
  {
    foundNewPossibleFace = false;
    Serial.println("Is moving");
    isMoving = true;
  }
}

#include "Vector3.h"
#include "Arduino.h"

Vector3::Vector3(float _x, float _y, float _z)
{
  x = _x;
  y = _y;
  z = _z;
}

Vector3::~Vector3()
{
}

void Vector3::printValues()
{
  /*Serial.println();
  Serial.print(x);
  Serial.print("\t");
  Serial.print(y);
  Serial.print("\t");
  Serial.print(z);
  Serial.print("\n");*/
}

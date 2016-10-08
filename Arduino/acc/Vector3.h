#ifndef Vector3_H
#define Vector3_H

#include "Arduino.h"

class Vector3 {
public:
  Vector3(float x, float y, float z);
  ~Vector3();
  void printValues();
  float x;
  float y;
  float z;
};

#endif

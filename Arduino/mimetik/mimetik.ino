//////////////////////////////////////////////////////
// Bluetooth Communication and state implementation //
//////////////////////////////////////////////////////
#include "Adafruit_TCS34725.h"
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include "Vector3.h"
#include "SensorColor.h"
#include "Pixels.h"
#include "States.h"

int x, y, z;
float xm, ym, zm, temp;
int xmin, ymin, zmin, xmax, ymax, zmax;
int old_color;
bool foundNewPossibleFace = false, firstEntry = false;
Vector3 *possibleFace;
bool isMoving = false;

Vector3 *currentAcc;
Vector3 *currentFace;
Vector3 *RGBvalues;
SensorColor *colorSensor;
Pixels *pixel_obj;
States *stateObj;

char values[255];

String kikubeColor, kikubeState;

/********* NUMBER OF PIXELS **********/
int numPixels = 6;
#define pixelPin 3;

int i = 0;


const int xpin = A3;                  // x-axis of the accelerometer
const int ypin = A2;                  // y-axis
const int zpin = A1;                  // z-axis (only on 3-axis models)

/********* Bluetooth settings *********/
SoftwareSerial BT(10, 11); // Bluetooth 10 RX, 11 TX.

/**************** Detect color settings *************************/

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
String str_detectedColor;

/*************** Pixels settings ****************/
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {

  Serial.begin(9600);
  // Start Bluetooth connection
  BT.begin(9600);
  //handshake
  //BT.write("a");

  // set acc inputs
  analogReference(EXTERNAL);  // sets the serial port to 9600
  x = analogRead(0);
  y = analogRead(1);
  z = analogRead(2);
  currentAcc = new  Vector3(x, y, z);
  currentFace = new Vector3(1, 1, 0);

  // initialize the sensor color object
  colorSensor = new SensorColor(tcs);

  //initialize the led strip object
  pixels.begin();
  pixel_obj = new Pixels();

  old_color = -1;
  kikubeColor = "none";
  kikubeState = "sleep";
  stateObj = new States();
}

void loop() {
  /**********************detection color***************/
  colorSensor->calculateColor(tcs);  //colorSensor->printResults();
  int detectedColor = colorSensor->detectColor();
  /******************end detection color****************/

  if (old_color != detectedColor) {
    old_color = detectedColor;

    switch (detectedColor)
    {
      case 0:
        BT.write("s:red?");
        //pixel_obj->RGBColor(pixels, 255, 0, 0);
        RGBvalues = new Vector3(255, 0, 0);
        kikubeColor = "red";
        stateObj->next_state = kikubeColor;
        stateObj->previousMillis = millis();
        break;
      case 1:
        BT.write("s:green?");
        //pixel_obj->RGBColor(pixels, 0, 255, 0);
        RGBvalues = new Vector3(0, 255, 0);
        kikubeColor = "green";
        stateObj->previousMillis = millis();
        stateObj->next_state = kikubeColor;
        break;
      case 2:
        BT.write("s:blue?");
        //pixel_obj->RGBColor(pixels, 0, 0, 255);
        RGBvalues = new Vector3(0, 0, 255);
        kikubeColor = "blue";
        stateObj->previousMillis = millis();
        stateObj->next_state = kikubeColor;
        break;
      default:
        BT.write("s:none?");
        break;
    }

    if (stateObj->current_state == "sleep") {
      stateObj->current_state = kikubeColor;
      stateObj->normalColorMode = true;
    }

    /*if (stateObj->ableToChangeColor)
      {
      stateObj->previousMillis = millis();
      stateObj->stateDirection = values;
      //stateObj->setNextState();
      stateObj->next_state = kikubeColor;
      }*/

    // END OF MESSAGE
    BT.write("#");
  }
  /**********************end bluetooth set/get data******************/


  /*********************Bluetooth set/get data**********************/
  if (BT.available()) {
    // get value from OF
    char value = BT.read();
    if (value != '#')
    {
      values[i++] = value;
    }

    if (value == '#')
    {
      clean();
    }
  }

  if (stateObj->next_state != "")
  {
    Serial.print("Current state: ");  Serial.println(stateObj->current_state);
    Serial.print("transition to: ");  Serial.println(stateObj->next_state);

    stateObj->beginStateTransition();

    //stateObj->current_state = stateObj->next_state;
  }
  
  //****************KIKUBE IN MODE OF TRANSITION OF COLOR********************
  if(stateObj->doingTransition)
  {
        //pixel_obj->RGBColor(pixels, 210, 0, 127);
        pixel_obj->transitionPixels(pixels, stateObj->next_state, stateObj->current_state);
  }
  /*****************************************************
    end direction set/get data
  */


  /*****************************************************
    acc analog reads
  */
  x = analogRead(0);       // read analog input pin 0
  delay(1);
  y = analogRead(1);       // read analog input pin 1
  delay(1);
  z = analogRead(2);       // read analog input pin 2
  delay(1);

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

  // send acc data
  String acc = "";
  acc = (String)currentAcc->x + ',' + (String)currentAcc->y + ',' + (String)currentAcc->z;
  //Serial.println(acc);
  //BT.write("");
  //currentAcc->printValues();

  // Modulate leds based on acc data
  //Serial.print(kikubeColor);

  if (stateObj->normalColorMode) {
    Serial.print("RGB values");
    Serial.print(RGBvalues->x); Serial.print(", ");
    Serial.print(RGBvalues->y); Serial.print(", ");
    Serial.print(RGBvalues->z); Serial.println(" ");
    pixel_obj->RGBColor(pixels, RGBvalues->x, RGBvalues->y, RGBvalues->z);
    //pixel_obj->ColorShift(pixels, kikubeColor, currentAcc->x);
    stateObj->doingTransition = false;
  }

}

float mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void detectFace(Vector3 * currentAcc)
{
  if (abs(currentAcc->x - currentFace->x) > 0.3 || abs(currentAcc->y - currentFace->y) > 0.3 || abs(currentAcc->y - currentFace->y) > 0.3 )
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
    //Serial.println("Is not moving");
    isMoving = true;
  }
}

// clean values
void clean()
{
  for (int cl = 0; cl <= i; cl++)
  {
    values[cl] = 0;
  }
  i = 0;
}

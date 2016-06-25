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
int xmin, ymin, zmin, xmax, ymax, zmax, xtemp, ytemp, ztemp;
const int xpin = A3;                  // x-axis of the accelerometer
const int ypin = A2;                  // y-axis
const int zpin = A1;                  // z-axis (only on 3-axis models)
#define zero_G 512.0
#define scale 102.3

int detectedColor;
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

int i = 0;

char newC;
char oldC;

/********* Bluetooth settings *********/
SoftwareSerial BT(10, 11); // Bluetooth 10 RX, 11 TX.

/**************** Color sensor settings *************************/
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

/*************** Pixels settings ****************/
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {

  Serial.begin(9600);
  // Start Bluetooth connection
  BT.begin(9600);

  // set acc inputs
  analogReference(EXTERNAL);
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
  //pixels.setBrightness(20);

  old_color = -1;
  stateObj = new States();
}

void loop() {

  /********************** Color sensor detection ***************/
  colorSensor->calculateColor(tcs);  //colorSensor->printResults();
  detectedColor = colorSensor->detectColor();

  if (old_color != detectedColor) {
    old_color = detectedColor;

    /**************** WRITE Bluetooth messages ***************/
    switch (detectedColor)
    {
      case 0:
        BT.write("s:red?");
        //pixel_obj->RGBColor(pixels, 255, 0, 0);
        RGBvalues = new Vector3(255, 0, 0);
        stateObj->next_state = 'r';
        stateObj->previousMillis = millis();
        break;
      case 1:
        BT.write("s:green?");
        //pixel_obj->RGBColor(pixels, 0, 255, 0);
        RGBvalues = new Vector3(0, 255, 0);
        stateObj->previousMillis = millis();
        stateObj->next_state = 'g';
        break;
      case 2:
        BT.write("s:blue?");
        //pixel_obj->RGBColor(pixels, 0, 0, 255);
        RGBvalues = new Vector3(0, 0, 255);
        stateObj->previousMillis = millis();
        stateObj->next_state = 'b';
        break;
      default:
        BT.write("s:none?");
        break;
    }

    if (stateObj->current_state == 's') //s means sleep state
    {
      stateObj->current_state = stateObj->next_state;
      stateObj->next_state = '0'; //0 means no state
      stateObj->stateMode = true;
    }

    // END OF MESSAGE
    BT.write("#");
  }


  /**************** READ Bluetooth messages ***************/
  if (BT.available()) {
    // get value from OF
    char value = BT.read();
    if (value != '#')
      values[i++] = value;

    if (value == '#')
      clean();
  }


  /**************** Transition mode ***************/
  if (stateObj->next_state != '0')
  {
    Serial.print("Current state: ");  Serial.println(stateObj->current_state);
    Serial.print("transition to: ");  Serial.println(stateObj->next_state);

    if (!stateObj->transitionMode) {
      pixel_obj->initTimeTransition = millis();
      pixel_obj->finishTimeTransition = millis() + stateObj->interval;
    }

    stateObj->beginStateTransition();
  }

  /**************** Pixels in transition ********************/
  if (stateObj->transitionMode)
  {
    Serial.print("init time is: "); Serial.print(pixel_obj->initTimeTransition);
    Serial.print(" finish time is: "); Serial.print(pixel_obj->finishTimeTransition);
    Serial.print(" millis are: "); Serial.print(millis());
    Serial.print(" mapped time is: ");
    float timeTransition = mapf(millis(), pixel_obj->initTimeTransition, pixel_obj->finishTimeTransition, 0, 1);
    while (timeTransition < 1)
    {
      timeTransition = mapf(millis(), pixel_obj->initTimeTransition, pixel_obj->finishTimeTransition, 0, 1);

      pixel_obj->transitionPixels(pixels, stateObj->next_state, stateObj->current_state, timeTransition);
      //pixel_obj->wupTransitionPixels(pixels, stateObj->next_state, stateObj->current_state,timeTransition);
    }
  }

  /**************** Read acc data ***************/
  x = analogRead(0); delay(1); y = analogRead(1); delay(1); z = analogRead(2); delay(1);

  xtemp = constrain(round(((float)x - zero_G) / scale * 127 + 127), 0, 255);
  ytemp = constrain(round(((float)y - zero_G) / scale * 127 + 127), 0, 255);
  ztemp = constrain(round(((float)z - zero_G) / scale * 127 + 127), 0, 255);

  currentAcc->x = mapf(xtemp, 0, 255, 0, 10);
  currentAcc->y = mapf(ytemp, 0, 255, 0, 10);
  currentAcc->z = mapf(ztemp, 0, 255, 0, 10);

  //detectFace(currentAcc);

  /*String acc = "";
    acc = (String)currentAcc->x + ',' + (String)currentAcc->y + ',' + (String)currentAcc->z;*/

  //currentAcc->printValues();

  /**************** State mode ***************/
  if (stateObj->stateMode) {
    //pixel_obj->RGBColor(pixels, RGBvalues->x, RGBvalues->y, RGBvalues->z);
    pixel_obj->ColorShift(pixels, stateObj->current_state, currentAcc->x, currentAcc->y,currentAcc->z, millis());
    stateObj->transitionMode = false;
  }



}

/**************** Functions ***************/

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

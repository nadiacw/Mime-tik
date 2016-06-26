//////////////////////////////////////////////////////
// Bluetooth Communication and state implementation //
//////////////////////////////////////////////////////
#include "Adafruit_TCS34725.h"
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
#include <Wire.h>

#include "Vector3.h"
#include "ColorSensor.h"
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
bool hasMoved = false;

Vector3 *currentAcc;
Vector3 *currentFace;
Vector3 *RGBvalues;
ColorSensor *colorSensorObj;
Pixels *pixelObj;
States *stateObj;

char values[255];

int i = 0;

unsigned long TIME;


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
  colorSensorObj = new ColorSensor(tcs);

  //initialize the led strip object
  pixels.begin();
  pixelObj = new Pixels();
  //pixels.setBrightness(20);

  old_color = -1;
  stateObj = new States();
  stateObj->current_state = 's';

}

void loop() {
  /**************** GET MILLIS ***************/
  TIME = millis();
  
    while(millis()<1000){
    colorSensorObj->calibrate(tcs);
    Serial.println("EOoooo");}
  
  Serial.println(stateObj->current_state);
  
  /**************** SLEEP state ***************/
  if (stateObj->current_state == 's') {
    while (stateObj->current_state == 's' && !hasMoved) {
      Serial.println("not moving!");
      ReadAcc();
      pixelObj->sleepPixels(pixels);
      hasMoved = isMoving;
    }
    Serial.println("moved!");
    hasMoved = true;
    pixelObj->RGBColor(pixels,200,200,200);
    pixels.setBrightness(255);
  }


  /********************** Color sensor detection ***************/

  ReadColorSensor();

  /**************** READ Bluetooth messages ***************/
  if (BT.available() && stateObj->current_state != 's') {
    // get value from OF
    char value = BT.read();
    if (value != '#')
      values[i++] = value;

    if (value == '#')
      clean();
  }


  /**************** Transition mode ***************/
  if (stateObj->next_state != '0' && stateObj->current_state != 's')
  {
    Serial.print("Current state: ");  Serial.println(stateObj->current_state);
    Serial.print("transition to: ");  Serial.println(stateObj->next_state);

    if (!stateObj->transitionMode) {
      pixelObj->initTimeTransition = TIME;
      pixelObj->finishTimeTransition = TIME + stateObj->interval;
    }

    stateObj->beginStateTransition();
  }

  /**************** Pixels in transition ********************/
  if (stateObj->transitionMode)
  {
    Serial.print("init time is: "); Serial.print(pixelObj->initTimeTransition);
    Serial.print(" finish time is: "); Serial.print(pixelObj->finishTimeTransition);
    //Serial.print(" millis are: "); Serial.print(millis());
    Serial.print(" mapped time is: ");
    float timeTransition = mapf(TIME, pixelObj->initTimeTransition, pixelObj->finishTimeTransition, 0, 1);
    while (timeTransition < 1)
    {
      x = analogRead(0); delay(1); y = analogRead(1); delay(1); z = analogRead(2); delay(1);

      xtemp = constrain(round(((float)x - zero_G) / scale * 127 + 127), 0, 255);
      ytemp = constrain(round(((float)y - zero_G) / scale * 127 + 127), 0, 255);
      ztemp = constrain(round(((float)z - zero_G) / scale * 127 + 127), 0, 255);

      currentAcc->x = mapf(xtemp, 0, 255, 0, 10);
      currentAcc->y = mapf(ytemp, 0, 255, 0, 10);
      currentAcc->z = mapf(ztemp, 0, 255, 0, 10);

      timeTransition = mapf(millis(), pixelObj->initTimeTransition, pixelObj->finishTimeTransition, 0, 1);

      pixelObj->transitionPixels(pixels, stateObj->next_state, stateObj->current_state, timeTransition, currentAcc->x, currentAcc->y, currentAcc->z, stateObj->interval);
      //pixelObj->wupTransitionPixels(pixels, stateObj->next_state, stateObj->current_state,timeTransition);
    }
  }

  /**************** Read acc data ***************/

  ReadAcc();

  /**************** State mode ***************/
  if (stateObj->stateMode) {
    //pixelObj->RGBColor(pixels, RGBvalues->x, RGBvalues->y, RGBvalues->z);
    pixelObj->ColorShift(pixels, stateObj->current_state, currentAcc->x, currentAcc->y, currentAcc->z, TIME);
    stateObj->transitionMode = false;
  }


  // END LOOP
}

/**************** Functions ***************/

void ReadColorSensor() {

  colorSensorObj->calculateColor(tcs);  colorSensorObj->printResults();
  detectedColor = colorSensorObj->detectColor();

  if (old_color != detectedColor) {
    old_color = detectedColor;

    /**************** WRITE Bluetooth messages ***************/
    switch (detectedColor)
    {
      case 0:
        BT.write("s:red?");
        //pixelObj->RGBColor(pixels, 255, 0, 0);
        RGBvalues = new Vector3(255, 0, 0);
        stateObj->next_state = 'r';
        stateObj->previousMillis = TIME;
        break;
      case 1:
        BT.write("s:green?");
        //pixelObj->RGBColor(pixels, 0, 255, 0);
        RGBvalues = new Vector3(0, 255, 0);
        stateObj->previousMillis = TIME;
        stateObj->next_state = 'g';
        break;
      case 2:
        BT.write("s:blue?");
        //pixelObj->RGBColor(pixels, 0, 0, 255);
        RGBvalues = new Vector3(0, 0, 255);
        stateObj->previousMillis = TIME;
        stateObj->next_state = 'b';
        break;
      default:
        BT.write("s:none?");
        break;
    }

    // FIRST COLOR DETECTION
    if (stateObj->current_state == 's') //s means sleep state
    {
      stateObj->current_state = stateObj->next_state;
      stateObj->next_state = '0'; //0 means no state
      stateObj->stateMode = true;
      pixels.setBrightness(255);
    }

    // END OF MESSAGE
    BT.write("#");
  }

}

void ReadAcc() {
  x = analogRead(0); delay(1); y = analogRead(1); delay(1); z = analogRead(2); delay(1);

  xtemp = constrain(round(((float)x - zero_G) / scale * 127 + 127), 0, 255);
  ytemp = constrain(round(((float)y - zero_G) / scale * 127 + 127), 0, 255);
  ztemp = constrain(round(((float)z - zero_G) / scale * 127 + 127), 0, 255);

  currentAcc->x = mapf(xtemp, 0, 255, 0, 10);
  currentAcc->y = mapf(ytemp, 0, 255, 0, 10);
  currentAcc->z = mapf(ztemp, 0, 255, 0, 10);

  detectFace(currentAcc);

  //currentAcc->printValues();
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
      temp = TIME;
      firstEntry = false;
      //Serial.print("Has detected possible face: ");
    }
    else
    {
      if (foundNewPossibleFace)
      {
        if (TIME - temp >= 1000) {
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

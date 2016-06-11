//////////////////////////////////////////////////////
// Bluetooth Communication and state implementation //
//////////////////////////////////////////////////////
#include "Adafruit_TCS34725.h"
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include "Vector3.h"
#include "SensorColor.h"

int x, y, z;
float xm, ym, zm, temp;
int xmin, ymin, zmin, xmax, ymax, zmax;

bool foundNewPossibleFace = false, firstEntry = false;
Vector3 *possibleFace;

Vector3 *currentAcc;
Vector3 *currentFace;
SensorColor *colorSensor;


const int xpin = A3;                  // x-axis of the accelerometer
const int ypin = A2;                  // y-axis
const int zpin = A1;                  // z-axis (only on 3-axis models)

/********* Bluetooth settings *********/
SoftwareSerial BT(10, 11); // Bluetooth 10 RX, 11 TX.

/**************** Detect color settings *************************/

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
String str_detectedColor;

/*************** Pixels settings ****************/
#define LED_PIN 3
char values[255];
int i = 0;
#define PIN            3
#define NUMPIXELS      6
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  
  Serial.begin(9600);
  
  // start the connection via bluetooth with tx 9600
  BT.begin(9600);

  // LEDs setup
  pixels.begin();
  
  // set acc inputs
  analogReference(EXTERNAL);  // sets the serial port to 9600
  x = analogRead(0);
  y = analogRead(1);
  z = analogRead(2);
  currentAcc = new  Vector3(x,y,z);
  currentFace = new Vector3(1, 1, 0);

  // initialize the sensor color object
  colorSensor = new SensorColor(tcs);

}

void loop() {

   /***************************************************
  * Bluetooth set/get data
  */
 if (BT.available()) {

    // get value from OF
    //if(BT.read()) {
      
      char value = BT.read();
      
       // change color depends on the received value
      if (value == 'r'){
        randomColor();
      }
      // restart values 
      value = 0;
      clean();
      
    //}
  } 
 
    
   /***************************************************
  * detection color
  */
   
  colorSensor->calculateColor(tcs);
  colorSensor->printResults();
  int detectedColor = colorSensor->detectColor();
  
  switch(detectedColor) 
  {
    case 0: str_detectedColor = "red"; break;
    case 1: str_detectedColor = "green"; break;
    case 2: str_detectedColor = "blue"; break;
  }
  
  char* buff = (char*) malloc(sizeof(char)*(str_detectedColor.length() + 1)); 
  str_detectedColor.toCharArray(buff, str_detectedColor.length() + 1); 
  /*****************************************************
  * end detection color
  */
  
  Serial.print("String to send to OF: ");
  Serial.println(buff);
  //Send to OF the detected color
  if (Serial.available()) {
    BT.write(buff);
    free(buff)
  }

   /*****************************************************
  * end bluetooth set/get data
  */
  
  /*****************************************************
  * acc analog reads
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

  //currentAcc->printValues();

  //delay(500);
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

// clean values
void clean()
{
  for (int cl = 0; cl <= i; cl++)
  {
    values[cl] = 0;
  }
  i = 0;
}

// set random color to send
void randomColor()
{
  int red = random(0, 255);
  int green = random(0, 255);
  int blue = random(0, 255);
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(red, green, blue));
    pixels.show();
  }
}


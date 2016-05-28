
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
#include <avr/power.h>
SoftwareSerial BT(10, 11); // Bluetooth 10 RX, 11 TX.
char values[255];
int i = 0;
#define PIN            3
#define NUMPIXELS      6
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  // BT setup
  Serial.begin(9600);
  BT.begin(9600);

  Serial.println("Setup");
  // LEDs setup
  pixels.begin();
  
  BT.print("a");
}

void loop() {
    Serial.print(BT.available());

  if (BT.available() > 0) {
    Serial.println("BT available");
    char value = BT.read();
    
    Serial.print("BT value: ");
    Serial.print(value);
    
    if (value == 'r'){
      randomColor();
    }
    value = 0;
    //values[i++] = value;

    //if (value == '\n')
    //{
     // Serial.print(values);

      //if (strstr(values, "r") != 0)
      //{
        //randomColor();
      //}
      //BT.write("\r");
      clean();
    //}
  }
  if (Serial.available())
    BT.write(Serial.read());
    
   //delay(3000);
}

void clean()
{
  for (int cl = 0; cl <= i; cl++)
  {
    values[cl] = 0;
  }
  i = 0;
}


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

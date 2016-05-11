// Watch video here: https://www.youtube.com/watch?v=BERHEVgaX40

/*
// TCS230 color recognition sensor 
// Sensor connection pins to Arduino are shown in comments

Color Sensor      Arduino
-----------      --------
 VCC               5V
 GND               GND
 s0                8
 s1                9
 s2                12
 s3                11
 OUT               10
 OE                GND
*/
const int s0 = 3;  
const int s1 = 4;  
const int s2 = 5;  
const int s3 = 6;  
const int out = 2;   
// LED pins connected to Arduino
//int redLed = 2;  
//int greenLed = 3;  
//int blueLed = 4;
// Variables  
int red = 0;  
int green = 0;  
int blue = 0;  
    
void setup()   
{  
  Serial.begin(9600); 
  pinMode(s0, OUTPUT);  
  pinMode(s1, OUTPUT);  
  pinMode(s2, OUTPUT);  
  pinMode(s3, OUTPUT);  
  pinMode(out, INPUT);  
//  pinMode(redLed, OUTPUT);  
//  pinMode(greenLed, OUTPUT);  
//  pinMode(blueLed, OUTPUT);  
  digitalWrite(s0, HIGH);  
  digitalWrite(s1, HIGH);  
}  
    
void loop() 
{  
  color();   
//
//  Serial.print(red, DEC);   
//  Serial.print(','); 
//  Serial.print(green, DEC);   
//  Serial.print(','); 
//  Serial.print(blue, DEC);  
//  Serial.print(','); 
//  Serial.println();  

  Serial.print("R:\t"); Serial.print(red,DEC);
  Serial.print("\tG:\t"); Serial.print(green,DEC);
  Serial.print("\tB:\t"); Serial.print(blue,DEC);
  Serial.print("\t");
  Serial.println();
  
  delay(300);   
//  digitalWrite(redLed, LOW);  
//  digitalWrite(greenLed, LOW);  
//  digitalWrite(blueLed, LOW);  
 }  
    
void color()  
{    
  digitalWrite(s2, LOW);  
  digitalWrite(s3, LOW);  
  //count OUT, pRed, RED  
  red = map(pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH),115,0,0,255);
//red =  pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  digitalWrite(s3, HIGH);  
  //count OUT, pBLUE, BLUE  
  blue = map(pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH),115,0,0,255);  
  //blue =  pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  digitalWrite(s2, HIGH);  
  //count OUT, pGreen, GREEN  
  green = map(pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH),115,0,0,255); 
 //green =  pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH); 
}

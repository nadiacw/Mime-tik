//////////////////////////////////////////////////////
// Bluetooth Communication and state implementation //
//////////////////////////////////////////////////////

#include <SoftwareSerial.h>
// BT connection via digital pins 10 (RX) and 11 (TX).
SoftwareSerial BT(10,11);

void setup() {
  // BT setup
  Serial.begin(9600);
  BT.begin(9600);

}

void loop() {

  // Get BT messages
  if(BT.available()){
    int value = BT.read();
    if(value == 'R'){
      // do cooooool things
    }
  }
  // Send BT messages
    BT.print("hola");

}

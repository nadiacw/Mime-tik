//
//  States,cpp
//  mimetik
//
//  Created by Sergi Armengol Suquet on 28/5/16.
//
//

#include "States.h"


States::States() {

  // initialize first state => ' sleep '
  this->current_state = "sleep";

}

States::~States() {

}

String States::getState() {
  return current_state;
}
String States::getDirection() {

  return stateDirection;
}

void States::setNextState() {

  Serial.print("Direction: "); Serial.println(this->stateDirection);
  if (this->current_state == "red")
  {
    this->next_state = this->stateDirection == "right" ? "blue" : "green";
  }
  else if (this->current_state == "green")
  {
    this->next_state = this->stateDirection == "right" ? "red" : "blue";
  }
  else if (this->current_state == "blue")
  {
    this->next_state = this->stateDirection == "right" ? "green" : "red";
  }
  else
  {
    Serial.println("Sorry, there's a problem with the direction ");
  }
}

void States::beginStateTransition()
{
    Serial.print("Previous millis: ");  Serial.println(previousMillis);
    if (millis() - previousMillis >= interval) {
      this->current_state = this->next_state;
      this->next_state = "";
      this->normalColorMode = true;
      doingTransition = false;
      Serial.println("Set up next state value");
    }
    else
    {
      normalColorMode = false;
      doingTransition = true;
    }
}







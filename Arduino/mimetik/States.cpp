#include "States.h"


States::States() {

  // initialize first state => ' sleep '
  this->current_state = 's';

}

States::~States() {

}

char States::getState() {
  return current_state;
}
char States::getDirection() {

  return stateDirection;
}

void States::setNextState() {

  Serial.print("Direction: "); Serial.println(this->stateDirection);
  if (this->current_state == 'r')
  {
    this->next_state = this->stateDirection == 'R' ? 'b' : 'g';
  }
  else if (this->current_state == 'g')
  {
    this->next_state = this->stateDirection == 'R' ? 'r' : 'b';
  }
  else if (this->current_state == 'b')
  {
    this->next_state = this->stateDirection == 'R' ? 'g' : 'r';
  }
  else
  {
    
    Serial.println("Sorry, there's a problem with the direction ");
  }
}

void States::beginStateTransition()
{
    //Serial.print("Previous millis: ");  Serial.print(previousMillis); Serial.print(" and moment is: "); Serial.println(millis() - previousMillis);
    if (millis() - previousMillis >= interval) {
      this->current_state = this->next_state;
      this->next_state = '0';
      stateMode = true;
      transitionMode = false;
      //Serial.println("Set up next state value");
    }
    else
    {
      stateMode = false;
      transitionMode = true;
    }
}







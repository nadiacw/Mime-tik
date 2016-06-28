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
    state = "sleep";
    finishTransition = 40000;
    
}

States::~States(){
    
}

string States::getState() {
    
    return state;
}

void States::setState(string _forward_state) {
    
    // define the direction that kikube has.
    defineDirection(state, _forward_state);
    
    if(state == "sleep")
    {
        // redefine state as previous state
        this->previousState = this->state;
        this->state = _forward_state;
        cout << "Actual state: " << state;
        cout << " Actual direction: " << direction << endl;
    }
    else{
        this->forwardState = _forward_state;
        cout << "Previous state: " << previousState;
        cout << "; Current state: " << state;
        cout << "; Forward state: " << forwardState;
        cout << " Actual direction: " << direction << endl;
        setTimeTransition(10, ofGetElapsedTimef());
    }
    
    /*this->previousState = this->state;
    this->state = forward_state;
    cout << "Actual state: " << state;
    cout << " Actual direction: " << direction << endl;*/
}

void States::defineDirection(string current_state, string forward_state) {
   
    if(current_state == "red" && forward_state == "blue") {
        
        this->direction = "right#";
        
    } else if (current_state == "blue" && forward_state == "red") {
        
        this->direction = "left#";
        
    }else if(current_state == "red" && forward_state == "green") {
        
        this->direction = "left#";
        
    }else if(current_state == "green" && forward_state == "red") {
        
        this->direction = "right#";
        
    }else if(current_state == "green" && forward_state == "blue") {
        
        this->direction = "left#";
        
    }else if(current_state == "blue" && forward_state == "green") {
        
        this->direction = "right#";
        
    } else if(current_state == "sleep" || forward_state == "sleep") {
        
        this->direction = "#";
    
    } else {
        this->direction = "error";
        cout << "Sorry, there's a problem with the direction " << endl;
    }
}

void States::setTimeTransition(int duration, float currentTime)
{
    isInTransition = true;
	transitionDuration = duration;
	initTransition = currentTime;
	finishTransition = initTransition + transitionDuration;
}

void States::updateStateFromTransition()
{
    this->previousState = this->state;
    this->state = this->forwardState;
    this->forwardState = "";
    this->direction = "#";
    this->isInTransition = false;
    printStates();
}

float States::getFinishTime()
{
    return finishTransition;
}

void States::printStates()
{
    cout << "Previous state: " << previousState;
    cout << "; Current state: " << state;
    cout << "; Forward state: " << forwardState;
    cout << " Actual direction: " << direction << endl;
}






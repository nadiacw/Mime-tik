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
    this->state = "sleep";
    
}

States::~States(){
    
}

string States::getState() {
    
    return state;
}

void States::setState(string forward_state) {
    
    // define the direction that kikube has.
    defineDirection(state, forward_state);
    
    // redefine state as previous state
	this->previousState = this->state;
    this->state = forward_state;
    cout << "Actual state: " << state;
    cout << " Actual direction: " << direction << endl;
}

void States::defineDirection(string previous_state, string forward_state) {
   
    if(previous_state == "red" && forward_state == "blue") {
        
        this->direction = "right#";
        
    } else if (previous_state == "blue" && forward_state == "red") {
        
        this->direction = "left#";
        
    }else if(previous_state == "red" && forward_state == "green") {
        
        this->direction = "left#";
        
    }else if(previous_state == "green" && forward_state == "red") {
        
        this->direction = "right#";
        
    }else if(previous_state == "green" && forward_state == "blue") {
        
        this->direction = "left#";
        
    }else if(previous_state == "blue" && forward_state == "green") {
        
        this->direction = "right#";
        
    } else if(previous_state == "sleep" || forward_state == "sleep") {
        
        this->direction = "#";
    
    } else {
        
        this->direction = "error";
        cout << "Sorry, there's a problem with the direction " << endl;
    }
    
    
}

void States::setTransition(int duration, float currentTime)
{
	transitionDuration = duration;
	initTransition = currentTime;
	finishTransition = initTransition + transitionDuration;
}






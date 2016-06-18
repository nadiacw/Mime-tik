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

void States::setState(string actual_state) {
    
    // define the direction that kikube has.
    defineDirection(state, actual_state);
    
    // redefine state as previous state 
    this->state = actual_state;
    cout << "Actual state: " << state;
    cout << " Actual direction: " << direction << endl;
}

void States::defineDirection(string previous_state, string actual_state) {
    
    
    
    if(previous_state == "red" && actual_state == "blue") {
        
        this->direction = "right";
        
    } else if (previous_state == "blue" && actual_state == "red") {
        
        this->direction = "left";
        
    }else if(previous_state == "red" && actual_state == "green") {
        
        this->direction = "left";
        
    }else if(previous_state == "green" && actual_state == "red") {
        
        this->direction = "right";
        
    }else if(previous_state == "green" && actual_state == "blue") {
        
        this->direction = "left";
        
    }else if(previous_state == "blue" && actual_state == "green") {
        
        this->direction = "right";
        
    } else if(previous_state == "sleep" || actual_state == "sleep") {
        
        this->direction = "";
    
    } else {
        
        this->direction = "error";
        cout << "Sorry, there's a problem with the direction " << endl;
    }
    
    
}






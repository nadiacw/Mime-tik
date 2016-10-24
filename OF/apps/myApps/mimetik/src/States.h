//
//  States.h
//  mimetik
//
//  Created by Sergi Armengol Suquet on 28/5/16.
//
//
#pragma once

#include "ofMain.h"

class States {
    
public:
    States();
    ~States();
    
    string getState();
    string getForwardState();
    void setState(string state);
    
    void defineDirection(string previous_state, string actual_state);
    void setTimeTransition(int duration, float currentTime);
    void updateStateFromTransition();
    
    string state;
    string previousState;
    string forwardState;
    string direction;
    
    float getFinishTime();
    bool isInTransition;
    
    void printStates();

    
private:
    int transitionDuration;
    int initTransition;
    int finishTransition;
};

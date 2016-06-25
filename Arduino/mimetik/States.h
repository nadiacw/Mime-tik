//
//  States.h
//  mimetik
//
//  Created by Sergi Armengol Suquet on 28/5/16.
//
//
#ifndef States_H
#define States_H

#include "Arduino.h"

class States {

    public:
        States();
        ~States();
        char getState();
        char getDirection();
        void setNextState();
        void beginStateTransition();
        char current_state;
        char stateDirection;
        char next_state;
        unsigned long previousMillis;
        bool stateMode = false;
        bool transitionMode = false;
        const long interval = 3000;

    

    
};

#endif 

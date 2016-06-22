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
        String getState();
        String getDirection();
        void setNextState();
        void beginStateTransition();
        String current_state;
        String stateDirection;
        String next_state;
        unsigned long previousMillis;
        bool normalColorMode = false;
        bool doingTransition = false;

    
    private:
      const long interval = 3000;

    
};

#endif 

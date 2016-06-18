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
        void setState(string state);
        void defineDirection(string previous_state, string actual_state);
        string state;
        string direction;
    
    private:
    
    
};

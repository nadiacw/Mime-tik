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
		void setTransition(int duration, float currentTime);
        string state;
		string previousState;
        string direction;
    float getFinishTime();
		

    private:
        bool isInTransition;
		int transitionDuration;
		int initTransition;
		int finishTransition;
};

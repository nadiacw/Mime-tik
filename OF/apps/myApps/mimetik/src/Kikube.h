//
//  States.h
//  mimetik
//
//  Created by Sergi Armengol Suquet on 28/5/16.
//
//
#pragma once

#include "ofMain.h"
#include "States.h"
#include "ofxAbletonLiveTrack.h"


class Kikube
{
public:
    Kikube(string const id = "0");
    ~Kikube();
    
    string getId();
    States kikube_state;
    
	void setTrack(ofxAbletonLiveTrack* track);
    int getPreviousIndexTrack();
	string direction;
    ofxAbletonLiveTrack* abletonTrack;
    ofxAbletonLiveTrack* abletonTrack_previous;
    bool firstIteration;
    int indexPrevious;
    
private:
    string ID;
    string color;
    string nextColor;
};


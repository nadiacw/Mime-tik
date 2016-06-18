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

class Kikube
{
public:
    Kikube(string const id = "0");
    ~Kikube();
    string getId();
    States kikube_state;
    
    
private:
    string ID;
    string color;
    string direction;
    string nextColor;
    
};


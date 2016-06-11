//
//  States.h
//  mimetik
//
//  Created by Sergi Armengol Suquet on 28/5/16.
//
//
#pragma once

#include "ofMain.h"

class Kikube
{
public:
    Kikube(string id);
    ~Kikube();
    string getId();
    string getColor();
    string getDirection();   
    
    
private:
    string ID;
    string color;
    string direction;
    string nextColor;
    
};


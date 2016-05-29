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
    Kikube(string id,string device,int baudrate);
    ~Kikube();
    string getId();
    string getColor();
    string getDirection();
    ofSerial kikubeSerial;
    
    
private:
    string ID;
    string color;
    string direction;
    string nextColor;
    
};


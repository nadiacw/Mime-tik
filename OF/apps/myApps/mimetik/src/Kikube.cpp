//
//  Kikube.cpp
//  mimetik
//
//  Created by Sergi Armengol Suquet on 28/5/16.
//
//

#include "Kikube.h"

Kikube::Kikube(string id, string deviceName, int tx){
    
    if(id != "") {
        this->ID = id;
    } else {
        perror ("The following error occurred");
    }
    
    kikubeSerial.setup(deviceName, tx);
    kikubeSerial.flush(); //flush the serial port once before we start
}

Kikube::~Kikube(){

}


// get the identifier of the kikube
string Kikube::getId() {
    
    return this->ID;
    
}

// get future color of the kikube
string Kikube::getColor() {
    
}

// get direction of the kikube state
string Kikube::getDirection() {
    
}

    

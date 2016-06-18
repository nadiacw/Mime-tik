//
//  Kikube.cpp
//  mimetik
//
//  Created by Sergi Armengol Suquet on 28/5/16.
//
//

#include "Kikube.h"

Kikube::Kikube(string id){
    
    if(id != "") {
        
        this->ID = id;
        
        // initialize the state of these kikube
        this->kikube_state = *new States();
        
    } else {
        perror ("The following error occurred");
    }
    

}

Kikube::~Kikube(){

}


// get the identifier of the kikube
string Kikube::getId() {
    
    return this->ID;
    
}


    

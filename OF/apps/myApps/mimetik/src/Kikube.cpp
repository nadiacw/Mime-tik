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
	this->firstTime = 1;

}

Kikube::~Kikube(){

}


// get the identifier of the kikube
string Kikube::getId() {
    
    return this->ID;
    
}

void Kikube::setTrack(ofxAbletonLiveTrack* track)
{	
		/*
		this->abletonTrack_previous->initTime = ofGetElapsedTimef();
		this->abletonTrack_previous->setFadeOut(ofGetElapsedTimef() + 3);
		*/

		this->abletonTrack = track;
		this->abletonTrack->initTime = ofGetElapsedTimef();
		this->abletonTrack->setFadeIn(ofGetElapsedTimef() + 3);
		this->abletonTrack_previous = this->abletonTrack;
		
}



    

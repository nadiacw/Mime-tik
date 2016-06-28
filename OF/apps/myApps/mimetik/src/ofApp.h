#pragma once

#include "ofMain.h"
#include "Kikube.h"
//#include "Utils.h"
#include "ofxGui.h"
#include "ofxAbletonLive.h"
#include "AbletonGui.h"

//this variable is how many mesages you are sending from Arduino
//this number should corrolate with your "bytesToSend" Array in your Ardunio Sketch
#define NUM_MSG_BYTES 11
#define NUM_KIKUBES 10


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
        void readArduinoData(ofSerial serial);
    
        ofSerial setupSerial(int i, int baud);
        vector<string> explode(char mssg[], char delim);
        vector<string> explode(string mssg, char delim);

        // Serial object
        ofSerial mySerial;
        ofSerial kikubeSerial[NUM_KIKUBES];
        int messageIndex;
        char bytesReceived[NUM_MSG_BYTES];

		//ABLETON
        bool workWithAbleton;
		ofxAbletonLive live;
		AbletonGui gui;
		int initTime;
		int endTime;
		int elapsedTime;
		int firstTime;
    
        // Tracks lists per state and direction
		ofxAbletonLiveTrack* waterTracks_right;
        vector<ofxAbletonLiveTrack*> waterTracks;
        ofxAbletonLiveTrack* waterTracks_left;
    
        ofxAbletonLiveTrack* fireTracks_right;
        vector<ofxAbletonLiveTrack*> fireTracks;
        ofxAbletonLiveTrack* fireTracks_left;
    
        ofxAbletonLiveTrack* forestTracks_right;
        vector<ofxAbletonLiveTrack*> forestTracks;
        ofxAbletonLiveTrack* forestTracks_left;
    
    
		int trackIndexTemp;
		vector<int> activeTracks;
		int recursiveGetIndex(vector<ofxAbletonLiveTrack*> listTrack);
        void setIndexTrack(Kikube *kikube);
		void setupAbletonGui();
    
    
private:
    map <string, ofSerial> Kikube_serial_hashmap;
    map <string, Kikube> Kikube_hashmap;
		
};

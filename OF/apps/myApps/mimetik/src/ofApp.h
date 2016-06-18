#pragma once

#include "ofMain.h"
#include "Kikube.h"
//#include "Utils.h"

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
        unsigned char send[5];
    
    
private:
    map <string, ofSerial> Kikube_serial_hashmap;
    map <string, Kikube> Kikube_hashmap;

		
};

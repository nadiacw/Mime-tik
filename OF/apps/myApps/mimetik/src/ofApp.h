#pragma once

#include "ofMain.h"
#include "Kikube.h"

//this variable is how many mesages you are sending from Arduino
//this number should corrolate with your "bytesToSend" Array in your Ardunio Sketch
#define NUM_MSG_BYTES 2
#define NUM_KIKUBES 10


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void readArduinoData(ofSerial serial);
    
    // SERIAL PART
    // Serial object
    ofSerial mySerial;
    ofSerial kikubeSerial[NUM_KIKUBES];
    bool bSendSerialMessage; // a flag for sending serial
    int bytesReceived[NUM_MSG_BYTES];
    // this will be used to count the number of frames
    // that have passed since the last time the app reads from the serial port
    int countCycles = 0;

    // we'll use this to count how many msgs have been received so far (for debugging)
    long numMsgRecvd;
    int total_devices;
    // SERIAL END
    int bytesRequired = 8;
    unsigned char bytes[8];
    int bytesRemaining;
    
    
    
    
private:
    map <string, ofSerial> Kikube_hashmap;

		
};

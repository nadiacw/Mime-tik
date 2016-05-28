#pragma once

#include "ofMain.h"
#include "Kikube.h"

//this variable is how many mesages you are sending from Arduino
//this number should corrolate with your "bytesToSend" Array in your Ardunio Sketch
#define NUM_MSG_BYTES 2
#define MAX_KIKUBES 10

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
    
    // SERIAL PART
    // Serial object
    ofSerial kikubesSerial[MAX_KIKUBES];
    ofSerial mySerial;
    
    // set this array's length to the number of msg bytes we are expecting to receive
    int bytesReceived[NUM_MSG_BYTES];
    int ids[MAX_KIKUBES];
    // we'll use this to count how many msgs have been received so far (for debugging)
    long numMsgRecvd;
    int total_devices;
    // SERIAL END
    
private:
    vector <Kikube> kikubeList;

		
};

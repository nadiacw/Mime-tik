#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    //SERIAL SETUP
    //mySerial.listDevices();
    int id = 0;
    string id_string = "";
    vector <ofSerialDeviceInfo> deviceList = mySerial.getDeviceList();
    for (int i = 0; i < deviceList.size(); i++) {
        
        if(deviceList[i].getDeviceName().find("cu.HC-06-DevB") == 0) {
            
            // creates the id about kikube
            id_string = "Kiku_"+to_string(id);
            
            // new kikube
            Kikube kik_temp = Kikube(id_string);
            
            // push new kikube into a list of kikubes
            kikubeList.push_back(kik_temp);
            
            //serial port must match what was declared in arduino Code ... 9600
            kikubesSerial[i].setup(deviceList[i].getDeviceName(), 9600);
            kikubesSerial[i].flush(); //flush the serial port once before we start

            id++;
        }
    }
    
    


}

//--------------------------------------------------------------
void ofApp::update(){
    
    //SERIAL PART
    // handshake "a"
    for (int con = 0; con<MAX_KIKUBES; con++) {
        
        
        if(kikubesSerial[con].available() >= NUM_MSG_BYTES) {
            
            //go through all received bytes
            for(int i=0; i<NUM_MSG_BYTES; i++){
                
                //read this byte
                int val = kikubesSerial[con].readByte();
                
                //store it in the array at index i
                bytesReceived[i] = val;
            }
            
            //Print
            // cout << "bytesReceived[0] = " << bytesReceived[0] << endl;
            //cout << "bytesReceived[1] = " << bytesReceived[1] << endl;
            // cout << "------------------------"<<endl;
            
            // flush: super important it clears the buffer
            // if buffer gets full you can't keep receiving values
            kikubesSerial[con].flush();
        } else {
            cout << " NO  bluetooth connection";
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    for (int con = 0; con<MAX_KIKUBES; con++) {
        
        kikubesSerial[con].writeByte('r');
        
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'r'){
        mySerial.writeByte('r');
        //ofBackground(ofRandom(255),ofRandom(255),ofRandom(255));
        ofBackground(255, 0, 0);
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == 'r'){
        ofBackground(255, 255, 255);
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    //mySerial.writeByte('r');
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

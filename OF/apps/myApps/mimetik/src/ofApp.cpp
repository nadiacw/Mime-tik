#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    //SERIAL SETUP
    //mySerial.listDevices();
    int id = 0;
    string id_string = "";
    vector <ofSerialDeviceInfo> deviceList = mySerial.getDeviceList(); //Every port
    for (int i = 0; i < deviceList.size(); i++) {
        
        if(deviceList[i].getDeviceName().find("cu.HC-06-DevB") == 0) {
            
            // creates the id about kikube
            id_string = "Kiku_"+to_string(id);
            
            // new kikube
            Kikube kik_temp = Kikube(id_string, deviceList[i].getDeviceName(), 9600);
            
            // push new kikube into a list of kikubes
            kikubeList.push_back(kik_temp);
            
            id++;
        }
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //SERIAL PART
    // handshake "a"
    for (int i = 0; i<kikubeList.size(); i++)
    {
        string bytesReceived = "";
        if(kikubeList[i].kikubeSerial.isInitialized())
        {
            //go through all received bytes
            for(int j=0; j<kikubeList[i].kikubeSerial.available(); j++)
            {
                //read this byte
                int val = kikubeList[i].kikubeSerial.readByte();
                
                //store it in the array at index i
                bytesReceived += val;
            }
            
            //Print
            cout << "string received = " << bytesReceived << endl;
            cout << "------------------------"<<endl;
            
            // flush: super important it clears the buffer
            // if buffer gets full you can't keep receiving values
            kikubeList[i].kikubeSerial.flush();
        }
        else
        {
            cout << kikubeList[i].getId() << " does not have sent any bytes";
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    for (int i = 0; i<kikubeList.size(); i++) {
        
        kikubeList[i].kikubeSerial.writeByte('r');
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

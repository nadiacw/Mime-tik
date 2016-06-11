#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    //SERIAL SETUP
    mySerial.listDevices();
    int id = 0;
    int serial_count = 0;
    string id_string = "";
    bSendSerialMessage = false;
    vector <ofSerialDeviceInfo> deviceList = mySerial.getDeviceList(); //Every port
    for (int i = 0; i < deviceList.size(); i++) {
        
        if(deviceList[i].getDeviceName().find("tty.HC-06-DevB") == 0) {

            // creates the id about kikube
            id_string = "Kiku_"+to_string(id);
            cout << id_string << endl;
            // new kikube
            Kikube kik_temp = Kikube(id_string);
            
            // this is set to the port where your device is connected
            kikubeSerial[id].setup(i, 9600);
            kikubeSerial[id].flush(); //flush the serial port once before we start
      
            // push new kikube into a list of kikubes
            Kikube_hashmap[id_string] = kikubeSerial[id];
            
            id++;
        }
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    //SERIAL PART
    // handshake "a"
    
    /*typedef std::map<std::string,ofSerial>::iterator it_type;
    for(it_type iterator = Kikube_hashmap.begin(); iterator != Kikube_hashmap.end(); iterator++)
    {
        
            if (bSendSerialMessage)
            {
                // send a handshake to the Arduino serial
                //iterator->second.writeByte('r');
                // make sure there's something to write all the data to
                unsigned char bytesReturned[NUM_MSG_BYTES];
                memset(bytesReturned, 0, NUM_MSG_BYTES);
                // keep reading bytes, until there's none left to read
                while( iterator->second.readBytes(bytesReturned, NUM_MSG_BYTES) > 0){
                    //read this byte
                    int val_read = iterator->second.readByte();
                
                }
            }
            // wait a few cycles before asking again
            bSendSerialMessage = false;
            countCycles++;
            if(countCycles == 5)
            {
                bSendSerialMessage = true;
                countCycles = 0;
            }
            iterator->second.flush();
       
    }*/
}

//--------------------------------------------------------------
void ofApp::draw(){
    /*for (int i = 0; i<kikubeList.size(); i++) {
        
        kikubeList[i].kikubeSerial.writeByte('r');
    }*/
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'r'){
        Kikube_hashmap["Kiku_0"].writeByte('r');
        //ofBackground(ofRandom(255),ofRandom(255),ofRandom(255));
        ofBackground(255, 0, 0);
    }
    
    if(key == 'g')
    {
        Kikube_hashmap["Kiku_1"].writeByte('r');
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

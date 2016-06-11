#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    //SERIAL SETUP
    mySerial.listDevices();
    int id = 0;
    string id_string = "";
    
    // we want to read 8 bytes
    bytesRemaining = bytesRequired;
    
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

    typedef std::map<std::string,ofSerial>::iterator it_type;
    for(it_type iterator = Kikube_hashmap.begin(); iterator != Kikube_hashmap.end(); iterator++)
    {
        
            // check for data
            if ( iterator->second.available() > 0)
            {
                cout << "available data: " << iterator->second.available() << endl;
                
                // try to read - note offset into the bytes[] array, this is so
                // that we don't overwrite the bytes we already have
                int bytesArrayOffset = bytesRequired - bytesRemaining;
                int result = iterator->second.readBytes( &bytes[bytesArrayOffset],bytesRemaining);
                cout << result << endl;
                
                // check for error code
                if ( result == OF_SERIAL_ERROR )
                {
                    // something bad happened
                    ofLog( OF_LOG_ERROR, "unrecoverable error reading from serial" );
                    break;
                }
                else if ( result == OF_SERIAL_NO_DATA )
                {
                    // nothing was read, try again
                    ofLog(OF_LOG_WARNING, "there is no data sent");
                }
                else
                {
                    // we read some data!
                    bytesRemaining -= result;
                }
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){

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
        ofBackground(0, 255, 0);
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

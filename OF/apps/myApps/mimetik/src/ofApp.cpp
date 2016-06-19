#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    messageIndex = 0;
    
    //SERIAL SETUP
    //mySerial.listDevices();
    int id = 0;
    string id_string = "";
    
    vector <ofSerialDeviceInfo> deviceList = mySerial.getDeviceList(); //Every port
    for (int i = 0; i < deviceList.size(); i++) {
        
        if(deviceList[i].getDeviceName().find("tty.HC-06-DevB") == 0) {

            // creates the id about kikube
            id_string = "Kiku_"+to_string(id);
            cout << id_string << endl;
            
            // new kikube
            Kikube kik_temp = Kikube(id_string);
            
            // push new kikube into a list of kikubes
            Kikube_hashmap[id_string] = kik_temp;
            
            // this is set to the port where your device is connected
            if(!kikubeSerial[id].setup(i, 9600))
            {
                kikubeSerial[id] = setupSerial(i, 9600);
            }
            
            kikubeSerial[id].flush(); //flush the serial port once before we start
      
            // push new kikube serial into a list of kikubes serials
            Kikube_serial_hashmap[id_string] = kikubeSerial[id];
            
            
            id++;
        }
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    int i = 0;
    //SERIAL PART

    map<string,ofSerial>::iterator iterator = Kikube_serial_hashmap.begin();
    
    for(; iterator != Kikube_serial_hashmap.end(); iterator++)
    {
        if ( iterator->second.available() > 0)
        {            
            bool hasFoundEndOfFile = false;
            //go through all received bytes
            for(int i = 0; i < (int)iterator->second.available(); i++)
            {
                //read one byte
                char val = iterator->second.readByte();

                if(val=='#')
                {
                    hasFoundEndOfFile = true;
                    messageIndex = 0;
                    break;
                }
                //store it in the array at index i
                bytesReceived[messageIndex] = val;
                messageIndex++;
                //cout << val;
            }
            if(!hasFoundEndOfFile)
            {
                //cout << " end of file not found " << bytesReceived << endl;
                return;
            }

            vector<string> explode_str = explode(bytesReceived,'?');
            for(int i = 0; i < explode_str.size(); i++)
            {
                
                if(explode_str[i][0] == 's')
                {
              
                    vector<string> state_part = explode(explode_str[i], ':');
                    
                    
                    // Define the new state at State class
                    cout << iterator->first << " ";
                    Kikube_hashmap[iterator->first].kikube_state.setState(state_part[1]);
                    
                    //reinit the send char array
                    unsigned char send[7];
                    for ( int j = 0; j< Kikube_hashmap[iterator->first].kikube_state.direction.size(); j++) {
                        send[j] = Kikube_hashmap[iterator->first].kikube_state.direction[j];
                    }
                    cout << "Send char: " << send << endl;
                   
                    Kikube_serial_hashmap[iterator->first].writeBytes(send,Kikube_hashmap[iterator->first].kikube_state.direction.size());
                }
            }

            // if end of file find ( s found ) flush it
            if(hasFoundEndOfFile) {
                iterator->second.flush();
                
            }
        } else {
            // start again without flush
            continue;
        
        }
        i++;
        
    }
    
}

ofSerial ofApp::setupSerial(int i, int baudrate)
{
    ofSerial ofS;
    cout << "Trying again to setup" << endl;
    if(!ofS.setup(i, baudrate))
    {
        ofS = setupSerial(i,baudrate);
    }
    else{return ofS;}
}

vector<string> ofApp::explode(char mssg[], char delim)
{
    vector<string> result;
    
    istringstream iss(mssg);
    for( string token; getline(iss,token,delim);)
    {
        result.push_back(move(token));
    }
    return result;
}

vector<string> ofApp::explode(string mssg, char delim)
{
    vector<string> result;
    
    istringstream iss(mssg);
    for( string token; getline(iss,token,delim);)
    {
        result.push_back(move(token));
    }
    return result;
}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'r'){
        unsigned char rand[6] = "rand#";
        Kikube_serial_hashmap["Kiku_0"].writeBytes(rand,4);
        cout << "keyboard: " << rand << endl;
        //ofBackground(ofRandom(255),ofRandom(255),ofRandom(255));
        ofBackground(255, 0, 0);
    }
    
    if(key == 'g')
    {
        Kikube_serial_hashmap["Kiku_1"].writeByte('r');
        ofBackground(0, 255, 0);
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == 'r'){
        ofBackground(255, 255, 255);
    }
}



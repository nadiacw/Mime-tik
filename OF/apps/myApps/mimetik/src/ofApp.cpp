#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    messageIndex = 0;
    
    //SERIAL SETUP
    mySerial.listDevices();
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
            
            // this is set to the port where your device is connected
            if(!kikubeSerial[id].setup(i, 9600))
            {
                kikubeSerial[id] = setupSerial(i, 9600);
            }
            
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
    typedef std::map<std::string,ofSerial>::iterator it_type;
    for(it_type iterator = Kikube_hashmap.begin(); iterator != Kikube_hashmap.end(); iterator++)
    {
        if ( iterator->second.available() > 0)
        {
            cout << "bytes received: " << iterator->second.available() << " ";
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
                cout << val;
            }
            if(!hasFoundEndOfFile)
            {
                cout << "end of file not found " << bytesReceived << endl;
                return;
            }

            
            vector<string> explode_str = explode(bytesReceived,'?');
            
            cout << "Bytes received: " << bytesReceived << endl;
            cout << "Exploded string pos 0: " << explode_str[0] << endl;
            cout << "Exploded string pos 1: " << explode_str[1] << endl;
            
            for(int i = 0; i < explode_str.size(); i++)
            {
                
                if(explode_str[i][0] == 's')
                {
                    cout << explode_str[i] << endl;
                    vector<string> state_part = explode(explode_str[i], ':');
                    cout << "Read state: " << state_part[1] << endl;
                }
            }

            // if end of file find ( s found ) flush it
            if(hasFoundEndOfFile) {
                iterator->second.flush();
                
            }
        } else {
            // start again without flush
            return;
        
        }
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



#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    messageIndex = 0;
	firstTime = 0;
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
            
            // push new kikube into a list of kikubes
            Kikube_hashmap[id_string] = kik_temp;
            
            // this is set to the port where your device is connected
            while(!kikubeSerial[id].setup(i, 9600))
            {
                kikubeSerial[id].setup(i, 9600);
            }
            
            kikubeSerial[id].flush(); //flush the serial port once before we start
      
            // push new kikube serial into a list of kikubes serials
            Kikube_serial_hashmap[id_string] = kikubeSerial[id];
            
            id++;
        }
    }

	live.setup("localhost", this, &ofApp::setupAbletonGui);
	
	//waits until connection is done
	while (live.getTracks().size() < 6)
	{
		live.update();
	}
	
	//sort all tracks depending on type
	map<int, ofxAbletonLiveTrack*>::iterator it = live.getTracks().begin();
	for (int i = 0; i < live.getTracks().size(); i++) {
		/*string trackNameType = explode(it->second->getName(),'_')[0];
		if (trackNameType == "w")
		{
			waterTracks.push_back(it->second);
		}*/
        cout << "Track name: ";
        cout << it->second->getName() << endl;

		it->second->setVolume(0.0);
		waterTracks.push_back(it->second);
		it++;
	}
	live.play();
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
                    setIndexTrack(&Kikube_hashmap[iterator->first]);
                   
					

                    //reinit the send char array
                    /*unsigned char send[7];
                    for ( int j = 0; j< Kikube_hashmap[iterator->first].kikube_state.direction.size(); j++) {
                        send[j] = Kikube_hashmap[iterator->first].kikube_state.direction[j];
                    }
                    cout << "Send char: " << send << endl;
                   
                    Kikube_serial_hashmap[iterator->first].writeBytes(send,Kikube_hashmap[iterator->first].kikube_state.direction.size());
                     */
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
    
	live.update();

	map<int, ofxAbletonLiveTrack*>::iterator it = live.getTracks().begin();
	float currentTime = ofGetElapsedTimef();
	for (; it != live.getTracks().end(); it++)
	{
		it->second->Update(currentTime);
	}
}
/***
 Recursive function to check if Serial is connected otherwise try it again
 ***/
ofSerial ofApp::setupSerial(int i, int baudrate)
{
    ofSerial ofS;
    cout << "Trying again to setup" << endl;
    if(!ofS.setup(i, baudrate))
    {   ofS.flush();
        ofS = setupSerial(i,baudrate);
    }
    else{return ofS;}
}



/***
 Get the previous state and calculate the new index track per kikube
 ***/
void ofApp::setIndexTrack(Kikube *kikube) {
    int currentIndex = NULL;
    int previousIndex = NULL;
    cout << "Num tracks: ";
    cout << waterTracks.size() << endl;
    //set track per kikube depends on the index track
    if (kikube->kikube_state.previousState.compare("blue") == 0 && kikube->kikube_state.direction.compare("#") == 0)
    {
        currentIndex = recursiveGetIndex(waterTracks);
        cout << "index is: " << currentIndex << endl;
        kikube->setTrack(live.getTrack(currentIndex));
       
    }
    else if (kikube->kikube_state.previousState.compare("blue") == 0 && kikube->kikube_state.direction.compare("left#") == 0)
    {
        currentIndex = recursiveGetIndex(waterTracks);
        cout << "index is: " << currentIndex << endl;
        kikube->setTrack(live.getTrack(currentIndex));
       
    }
    else if (kikube->kikube_state.previousState.compare("blue") == 0 && kikube->kikube_state.direction.compare("right#") == 0)
    {
        currentIndex = recursiveGetIndex(waterTracks);
        cout << "index is: " << currentIndex << endl;
        kikube->setTrack(live.getTrack(currentIndex));
    } else if (kikube->kikube_state.previousState.compare("red") == 0 && kikube->kikube_state.direction.compare("#") == 0)
    {
        currentIndex = recursiveGetIndex(waterTracks);
        cout << "index is: " << currentIndex << endl;
        kikube->setTrack(live.getTrack(currentIndex));
    }else if (kikube->kikube_state.previousState.compare("red") == 0 && kikube->kikube_state.direction.compare("left#") == 0)
    {
        currentIndex = recursiveGetIndex(waterTracks);
        cout << "index is: " << currentIndex << endl;
        kikube->setTrack(live.getTrack(currentIndex));
    } else if (kikube->kikube_state.previousState.compare("red") == 0 && kikube->kikube_state.direction.compare("right#") == 0)
    {
        currentIndex = recursiveGetIndex(waterTracks);
        cout << "index is: " << currentIndex << endl;
        kikube->setTrack(live.getTrack(currentIndex));
    }
    else if (kikube->kikube_state.previousState.compare("green") == 0 && kikube->kikube_state.direction.compare("#") == 0)
    {
        currentIndex = recursiveGetIndex(waterTracks);
        cout << "index is: " << currentIndex << endl;
        kikube->setTrack(live.getTrack(currentIndex));
    } else if (kikube->kikube_state.previousState.compare("green") == 0 && kikube->kikube_state.direction.compare("left#") == 0)
    {
        currentIndex = recursiveGetIndex(waterTracks);
        cout << "index is: " << currentIndex << endl;
        kikube->setTrack(live.getTrack(currentIndex));
    }
    else if (kikube->kikube_state.previousState.compare("green") == 0 && kikube->kikube_state.direction.compare("right#") == 0)
    {
        currentIndex = recursiveGetIndex(waterTracks);
        cout << "index is: " << currentIndex << endl;
        kikube->setTrack(live.getTrack(currentIndex));
    }
    
    //When there're some previous tracks
    previousIndex = kikube->getPreviousIndexTrack();
    cout << "Previous index: " << previousIndex << endl;
    if(kikube->indexPrevious != -1){
        
        // delete active index of the previous track
        activeTracks.erase(std::remove(activeTracks.begin(), activeTracks.end(), previousIndex), activeTracks.end());
    }
    
    cout << activeTracks.size() << endl;
    for(std::vector<int>::iterator it_active = activeTracks.begin(); it_active != activeTracks.end(); it_active++)
    {
        cout << *it_active << endl;
    }
}


/***
 Split an string into an array by a delimeter
 ***/
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


/***
 Split an array char into an array by a delimeter
 ***/
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


/***
 Recursive function to check if the current index is used by another kikube
 ***/
int ofApp::recursiveGetIndex(vector<ofxAbletonLiveTrack*> listTrack) {
    
    int tempVectorIndex = ofRandom(0, listTrack.size());
    int trackIndex = waterTracks[tempVectorIndex]->getTrackIndex();
    
    if (std::find(activeTracks.begin(), activeTracks.end(),trackIndex) != activeTracks.end()) {
        return recursiveGetIndex(listTrack);
    }
    else {
        activeTracks.push_back(trackIndex);
        return trackIndex;
    }
    
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

	if (key == 'd') {
		map<int, ofxAbletonLiveTrack*>::iterator it = std::next(live.getTracks().begin(), 1);
		it->second->initTime = ofGetElapsedTimef();
		it->second->setFadeOut(ofGetElapsedTimef() + 10);
		cout << "fading out" << endl;
	}

	if (key == 'u') {
		map<int, ofxAbletonLiveTrack*>::iterator it = std::next(live.getTracks().begin(), 1);
		it->second->initTime = ofGetElapsedTimef();
		it->second->setFadeIn(ofGetElapsedTimef() + 5);
		cout << "fading in" << endl;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == 'r'){
        ofBackground(255, 255, 255);
    }
}

//--------------------------------------------------------------
void ofApp::setupAbletonGui()
{
	// setupAbletonGui() was passed to the live setup method and will
	// automatically be called when live has finished parsing. at that
	// point we can generate a gui
	gui.setup(&live);
}


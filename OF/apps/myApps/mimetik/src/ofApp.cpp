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
	while (live.getTracks().size() < 34)
	{
		live.update();
	}
	
	//sort all tracks depending on type
	map<int, ofxAbletonLiveTrack*>::iterator it = live.getTracks().begin();
	for (int i = 0; i < live.getTracks().size(); i++) {
        
        // Select in which list has to be the track depends on the name
		vector<string> trackNameType = explode(it->second->getName(),'_');

        if(trackNameType[0] == "r")
        {
            fireTracks.push_back(it->second);
        }
        else if(trackNameType[0] == "g")
        {
            forestTracks.push_back(it->second);
        }
        else if (trackNameType[0] == "b")
        {
            waterTracks.push_back(it->second);
        }
        else if(trackNameType[0] == "t")
        {
            if(trackNameType[1] == "r")
            {
                if(trackNameType[2] == "r") fireTracks_right = it->second;
                if(trackNameType[2] == "l") fireTracks_left = it->second;
            }
            else if(trackNameType[1] == "g")
            {
                if(trackNameType[2] == "r") forestTracks_right = it->second;
                if(trackNameType[2] == "l") forestTracks_left = it->second;
                
            }
            else if(trackNameType[1] == "b")
            {
                if(trackNameType[2] == "r") waterTracks_right = it->second;
                if(trackNameType[2] == "l") waterTracks_left = it->second;
            }
        }
      
        cout << "Track name: " << it->second->getName() << endl;

		it->second->setVolume(0.0);
        
        if(it->second->getName() == "sleep")
        {
            sleepTrack = it->second;
            it->second->setVolume(0.85);
            sleepActive = true;
        }
        
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
    
    map<string,Kikube>::iterator it_kikube = Kikube_hashmap.begin();
    int sleepCounter = 0;
    for (; it_kikube != Kikube_hashmap.end(); it_kikube++)
    {
        if(it_kikube->second.kikube_state.isInTransition  && it_kikube->second.kikube_state.getFinishTime() < ofGetElapsedTimef())
        {
            cout << "updating state of: " << it_kikube->second.getId() << endl;
            it_kikube->second.kikube_state.updateStateFromTransition();
            setIndexTrack(&Kikube_hashmap[it_kikube->first]);
        }
        if(it_kikube->second.kikube_state.state == "sleep")
        {
            sleepCounter++;
        }
    }
    if(sleepCounter == Kikube_hashmap.size() && abs(sleepTrack->getVolume()) < 0.01 && !sleepActive)
    {
        sleepTrack->initTime = ofGetElapsedTimef();
        sleepTrack->setFadeIn(ofGetElapsedTimef() + 1);
        sleepActive = true;
        cout << "setting fade in of sleep track " << endl;
    }
    
	live.update();
    
	map<int, ofxAbletonLiveTrack*>::iterator it = live.getTracks().begin();
	float currentTime = ofGetElapsedTimef();
	for (; it != live.getTracks().end(); it++)
	{
		it->second->Update(currentTime);
 	}
}
//END UPDATE



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
    
    //set track per kikube depends on the index track
    if (kikube->kikube_state.state.compare("blue") == 0 && kikube->kikube_state.direction.compare("#") == 0)
    {
        currentIndex = recursiveGetIndex(waterTracks);
        cout << "Track name: " << live.getTrack(currentIndex)->getName() << endl;
        kikube->setTrack(live.getTrack(currentIndex));
    }
    else if (kikube->kikube_state.state.compare("blue") == 0 && kikube->kikube_state.direction.compare("left#") == 0)
    {
        //FIND IF TRACK IS ALREADY ACTIVE
        if (std::find(activeTracks.begin(), activeTracks.end(),waterTracks_left->getTrackIndex()) != activeTracks.end()) {
            return;
        }
        kikube->setTrack(live.getTrack(waterTracks_left->getTrackIndex()));
        cout << "Track name: " << live.getTrack(waterTracks_left->getTrackIndex())->getName() << endl;
        activeTracks.push_back(waterTracks_left->getTrackIndex());
    }
    else if (kikube->kikube_state.state.compare("blue") == 0 && kikube->kikube_state.direction.compare("right#") == 0)
    {
        //FIND IF TRACK IS ALREADY ACTIVE
        if (std::find(activeTracks.begin(), activeTracks.end(),waterTracks_right->getTrackIndex()) != activeTracks.end()) {
            return;
        }
        kikube->setTrack(live.getTrack(waterTracks_right->getTrackIndex()));
        cout << "Track name: " << live.getTrack(waterTracks_right->getTrackIndex())->getName() << endl;
        activeTracks.push_back(waterTracks_right->getTrackIndex());
    }
    else if (kikube->kikube_state.state.compare("red") == 0 && kikube->kikube_state.direction.compare("#") == 0)
    {
        currentIndex = recursiveGetIndex(fireTracks);
        cout << "Track name: " << live.getTrack(currentIndex)->getName() << endl;
        kikube->setTrack(live.getTrack(currentIndex));
    }
    else if (kikube->kikube_state.state.compare("red") == 0 && kikube->kikube_state.direction.compare("left#") == 0)
    {
        //FIND IF TRACK IS ALREADY ACTIVE
        if (std::find(activeTracks.begin(), activeTracks.end(),fireTracks_left->getTrackIndex()) != activeTracks.end()) {
            return;
        }
        kikube->setTrack(live.getTrack(fireTracks_left->getTrackIndex()));
        cout << "Track name: " << fireTracks_left->getName() << endl;
        activeTracks.push_back(fireTracks_left->getTrackIndex());
    }
    else if (kikube->kikube_state.state.compare("red") == 0 && kikube->kikube_state.direction.compare("right#") == 0)
    {
        //FIND IF TRACK IS ALREADY ACTIVE
        if (std::find(activeTracks.begin(), activeTracks.end(),fireTracks_right->getTrackIndex()) != activeTracks.end()) {
            return;
        }
        kikube->setTrack(live.getTrack(fireTracks_right->getTrackIndex()));
        cout << "Track name: " << fireTracks_right->getName() << endl;
        activeTracks.push_back(fireTracks_right->getTrackIndex());
    }
    else if (kikube->kikube_state.state.compare("green") == 0 && kikube->kikube_state.direction.compare("#") == 0)
    {
        currentIndex = recursiveGetIndex(forestTracks);
        cout << "Track name: " << live.getTrack(currentIndex)->getName() << endl;
        kikube->setTrack(live.getTrack(currentIndex));
    }
    else if (kikube->kikube_state.state.compare("green") == 0 && kikube->kikube_state.direction.compare("left#") == 0)
    {
        //FIND IF TRACK IS ALREADY ACTIVE
        if (std::find(activeTracks.begin(), activeTracks.end(),forestTracks_left->getTrackIndex()) != activeTracks.end()) {
            return;
        }
        kikube->setTrack(live.getTrack(forestTracks_left->getTrackIndex()));
        cout << "Track name: " << live.getTrack(forestTracks_left->getTrackIndex())->getName() << endl;
        activeTracks.push_back(forestTracks_left->getTrackIndex());
    }
    else if (kikube->kikube_state.state.compare("green") == 0 && kikube->kikube_state.direction.compare("right#") == 0)
    {
        //FIND IF TRACK IS ALREADY ACTIVE
        if (std::find(activeTracks.begin(), activeTracks.end(),forestTracks_right->getTrackIndex()) != activeTracks.end()) {
            return;
        }
        kikube->setTrack(live.getTrack(forestTracks_right->getTrackIndex()));
        cout << "Track name: " << live.getTrack(forestTracks_right->getTrackIndex())->getName() << endl;
        activeTracks.push_back(forestTracks_right->getTrackIndex());
    }
    else if(kikube->kikube_state.state.compare("sleep") == 0)
    {
        /*kikube->abletonTrack->initTime = ofGetElapsedTimef();
        kikube->abletonTrack->setFadeOut(ofGetElapsedTimef()+3);*/
        kikube->KikubeFadeOut();
        cout << "received sleep message: should fade out previous track" << endl;
    }
    
    //check if sleep track is on; if it is: set a fade out
    if(abs(0.85 - sleepTrack->getVolume()) < 0.1 && kikube->kikube_state.state.compare("sleep") != 0 && sleepActive)
    {
        sleepTrack->initTime = ofGetElapsedTimef();
        sleepTrack->setFadeOut(ofGetElapsedTimef()+3);
        sleepActive = false;
        cout << "sleep track is ON and should be faded out" << endl;
    }
    
    //When there're some previous tracks
    previousIndex = kikube->getPreviousIndexTrack();
    cout << "Previous index: " << previousIndex << endl;
    if(kikube->indexPrevious != -1) {
        // delete active index of the previous track
        activeTracks.erase(std::remove(activeTracks.begin(), activeTracks.end(), previousIndex), activeTracks.end());
    }
    
    cout << "Active tracks are: " << activeTracks.size() << ": ";
    for(std::vector<int>::iterator it_active = activeTracks.begin(); it_active != activeTracks.end(); it_active++)
    {
        cout << *it_active << ", ";
    }
    cout << endl;
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
    int trackIndex = listTrack[tempVectorIndex]->getTrackIndex();
    
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
    
    if(key == 'i')
    {
        unsigned char rand[3] = "i#";
        map<string,ofSerial>::iterator iterator = Kikube_serial_hashmap.begin();
        for(; iterator != Kikube_serial_hashmap.end(); iterator++)
        {
            iterator->second.writeBytes(rand,2);
        }
        
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

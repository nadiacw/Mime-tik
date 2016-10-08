//
//  SerialThread.cpp
//  mimetik
//
//  Created by Nadia Campo Woyuk on 07/10/16.
//
//

#include "SerialThread.h"

class SerialThread : public ofThread {
    
    // the thread function
    void SerialThread::threadedFunction() {
        
        // start
        
        while(isThreadRunning()) {
            
            cam.update();
            if(cam.isFrameNew()) {
                
                // load the image
                image.setFromPixels(cam.getPixels());
            }
        }
        
        // done
    }
    
    ofVideoGrabber cam; // the cam
    ofImage image;
};

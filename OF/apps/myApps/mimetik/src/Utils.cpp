//
//  Utils.cpp
//  mimetik
//
//  Created by Sergi Armengol Suquet on 12/6/16.
//
//

#include "Utils.h"


vector<string> explode(char mssg[], char delim)
{
    vector<string> result;
    
    istringstream iss(mssg);
    for( string token; getline(iss,token,delim);)
    {
        result.push_back(move(token));
    }
    return result;
}
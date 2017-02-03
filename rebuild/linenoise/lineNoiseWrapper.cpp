//

//  lineNoiseWrapper.cpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 31/01/17.
//  Copyright © 2017 self. All rights reserved.
//

#include "lineNoiseWrapper.hpp"
#include "linenoise.h"
#include <stdlib.h>





std::string LineNoiseWrapper::getLine(std::string prompt)
{
    std::string returnstring;
    char * result=linenoise(prompt.c_str());
    
    if (result) {
        
        returnstring = result;
        linenoiseHistoryAdd(result);
        
    }
    
    linenoiseFree(result);
    return returnstring;

}


const char * LineNoiseWrapper::linenoiseHistoryCallback(int direction, const char * oldline)
{
    return NULL;
    
}

LineNoiseWrapper::LineNoiseWrapper()
{
    const char *homedir;
    
    if ((homedir = getenv("HOME")) == NULL) {
    }

    
    linenoiseHistoryLoad((std::string("")+ homedir+"/.rebuild.linenoise.txt").c_str());
    
    linenoiseSetHistoryCallback(LineNoiseWrapper::linenoiseHistoryCallback);
}

LineNoiseWrapper::~LineNoiseWrapper()
{
    const char *homedir;
    
    if ((homedir = getenv("HOME")) == NULL) {
    }
    //linenoiseHistorySave((std::string("")+ homedir+"/.rebuild.linenoise.txt").c_str());

}
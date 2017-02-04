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
    history.ReInit();
    char * result=linenoise(prompt.c_str());
    history.ReInitDone();

    if (result) {
        
        returnstring = result;
        if(returnstring!="")
            history.Add(result);
       // linenoiseHistoryAdd(result);
        
        
    }
    
    linenoiseFree(result);
    return returnstring;

}


const char * LineNoiseWrapper::linenoiseHistoryCallback(int direction, const char * oldline,void * context)
{
    LineHistory * history = static_cast<LineHistory *>(context);
    bool success;
    std::string result =history->Edit(oldline,(direction==1)?
                                     LineHistory::MoveDirection::prev
                                     :LineHistory::MoveDirection::next,success);
    
    return (success)?result.c_str():nullptr;
    
}

LineNoiseWrapper::LineNoiseWrapper()
{
    const char *homedir;
    
    if ((homedir = getenv("HOME")) == NULL) {
    }
    
    
    history.Load(std::string("")+ homedir+"/.rebuild.linenoise.txt");
    
    //linenoiseHistoryLoad((std::string("")+ homedir+"/.rebuild.linenoise.txt").c_str());
    
    linenoiseSetHistoryCallback(LineNoiseWrapper::linenoiseHistoryCallback,this);
}

LineNoiseWrapper::~LineNoiseWrapper()
{
    const char *homedir;
    
    if ((homedir = getenv("HOME")) == NULL) {
    }
    //linenoiseHistorySave((std::string("")+ homedir+"/.rebuild.linenoise.txt").c_str());
    
    history.Save(std::string("")+ homedir+"/.rebuild.linenoise.txt");


}
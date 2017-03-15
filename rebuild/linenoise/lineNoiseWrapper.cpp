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




std::string LineNoiseWrapper::getLineWithHistory(std::string prompt,LineHistory & inhistory)
{
    localHistory = &inhistory;
    
    std::string returnstring;
    inhistory.ReInit();
    char* result = linenoise(prompt.c_str());
    inhistory.ReInitDone();
    
    if (result) {
        returnstring = result;
    }
    
    if (errno == EAGAIN)
        status = EStatus::ctrl_c;
    
    linenoiseFree(result);
    
    return returnstring;
    
}



///Returns the line read by prompt with default history
std::string
LineNoiseWrapper::getLine(std::string prompt)
{
    return getLineWithHistory(prompt, defaultHistory);
}

 char*
LineNoiseWrapper::linenoiseHistoryCallbackStatic(int direction, const char* oldline,
    void* context)
{
    LineNoiseWrapper * that= static_cast<LineNoiseWrapper*>(context);
    
    
    bool success;
    std::string result = that->LinenoiseHistoryCallback(direction, oldline, success).c_str();

    return (success) ? strdup(result.c_str()) : nullptr;
}


std::string LineNoiseWrapper::LinenoiseHistoryCallback(int direction, std::string oldline,bool &  success)
{
    std::string result = localHistory->Edit(oldline, (direction == 1) ? LineHistory::MoveDirection::prev
                                      : LineHistory::MoveDirection::next,
                                      success);
    
    return result;
    
}


LineNoiseWrapper::LineNoiseWrapper(LineHistory & linehistory)
:defaultHistory(linehistory)
{
    linenoiseSetHistoryCallback(LineNoiseWrapper::linenoiseHistoryCallbackStatic, this);
}

LineNoiseWrapper::~LineNoiseWrapper()
{
}


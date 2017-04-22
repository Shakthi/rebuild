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
    
    linenoiseOptions option;
    linenoiseResults results;
    results.ctrlKey=0;
    results.printNewln = false;
    option.printNewln = false;
    ctrlKey =0;
    
    char* result = linenoise(prompt.c_str(),&option,&results);
    inhistory.ReInitDone();
    
    if(option.printNewln==false && results.printNewln == false)
        printf("\n");
    
    
    if (result) {
        returnstring = result;
    }
    
    
    if (errno == EAGAIN)
        status = ExitStatus::ctrl_c;
    
    else if (results.ctrlKey!=0)
    {
        status = ExitStatus::ctrl_X;
        ctrlKey=results.ctrlKey;
    }
        
    
    if(loadedBuffer != returnstring)
        mstatus = EModificationStatus::edited;
        
    
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
    
    mstatus = EModificationStatus::history;
    loadedBuffer=result;
    
    return result;
    
}

void LineNoiseWrapper::ClearScreen()
{
    linenoiseClearScreen();
}

LineNoiseWrapper::LineNoiseWrapper(LineHistory & linehistory)
:defaultHistory(linehistory)
{
    linenoiseSetHistoryCallback(LineNoiseWrapper::linenoiseHistoryCallbackStatic, this);
}

LineNoiseWrapper::~LineNoiseWrapper()
{
}


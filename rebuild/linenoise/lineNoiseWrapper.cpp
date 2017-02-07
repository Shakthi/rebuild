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
    
    }
    
    
    if(errno == EAGAIN)
        status = EStatus::ctrl_c;
        
    
    linenoiseFree(result);
    return returnstring;

}


const char * LineNoiseWrapper::linenoiseHistoryCallback(int direction, const char * oldline,void * context)
{
    LineHistory  & history = static_cast<LineNoiseWrapper *>(context)->history;
    bool success;
    std::string result =history.Edit(oldline,(direction==1)?
                                     LineHistory::MoveDirection::prev
                                     :LineHistory::MoveDirection::next,success);
    
    return (success)?result.c_str():nullptr;
    
}

LineNoiseWrapper::LineNoiseWrapper()
{
    
    linenoiseSetHistoryCallback(LineNoiseWrapper::linenoiseHistoryCallback,this);
}

LineNoiseWrapper::~LineNoiseWrapper()
{
}



nlohmann::json LineNoiseWrapper::ToJson()
{
    return history.ToJson();
}
void LineNoiseWrapper::FromJson(nlohmann::json input)
{
    history.FromJson(input);
}


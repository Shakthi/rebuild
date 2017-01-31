//

//  lineNoiseWrapper.cpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 31/01/17.
//  Copyright © 2017 self. All rights reserved.
//

#include "lineNoiseWrapper.hpp"
#include "linenoise.h"

std::string LineNoiseWrapper::getLine(std::string prompt)
{
    std::string returnstring;
    char * result=linenoise(prompt.c_str());
    
    if (result) {
        
        returnstring = result;
    }
    
    linenoiseFree(result);
    return returnstring;

}
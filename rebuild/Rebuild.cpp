//
//  Rebuild.cpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 31/01/17.
//  Copyright © 2017 self. All rights reserved.
//

#include "Rebuild.hpp"

#include "lineNoiseWrapper.hpp"

#include <iostream>


Rebuild::Rebuild():exitStatus(exitStatusRIP),alive(true)
{
    Load();
    lineNoiseWrapper = new LineNoiseWrapper();
}


void Rebuild::Load()
{
    std::clog<<"Loading..."<<std::endl;
}


Rebuild::~Rebuild()
{
    delete lineNoiseWrapper;

}



void Rebuild::RunStep()
{
    
    std::string answer=lineNoiseWrapper->getLine("rebuild:");
    if(answer!="")
    {
        std::cout<<"rebuild>"<<answer<<std::endl;
    }
    else
        alive = false;
    

}


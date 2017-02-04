//
//  Rebuild.cpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 31/01/17.
//  Copyright © 2017 self. All rights reserved.
//

#include "Rebuild.hpp"

#include "lineNoiseWrapper.hpp"
#include "BasicStepProccessor.hpp"

#include <iostream>


Rebuild::Rebuild():exitStatus(exitStatusRIP),alive(true)
{
    Load();
    lineNoiseWrapper = new LineNoiseWrapper();
    processorStack.push(new BasicStepProcessor(this));

}


void Rebuild::Load()
{
    std::clog<<"Loading..."<<std::endl;
}


Rebuild::~Rebuild()
{
    delete lineNoiseWrapper;

}


void Rebuild::exitProcessing()
{
    StepProcessor * last = processorStack.top();
    processorStack.pop();
    delete last;
}

void Rebuild::addNewProcessing(StepProcessor* stepProcessor)
{
    processorStack.push(stepProcessor);
}



void Rebuild::RunStep()
{
    
    
    if (processorStack.empty()) {
        alive = false;
        return;
    }
    
    
    StepProcessor  * stepProcessor = processorStack.top();
    
    stepProcessor->RunStep();
     

}


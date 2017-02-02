//
//  StepProcessor.hpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 02/02/17.
//  Copyright © 2017 self. All rights reserved.
//

#ifndef StepProcessor_hpp
#define StepProcessor_hpp

#include <stdio.h>


class StepProcessor {
    
protected:
    class Rebuild * rebuild;
public:
    
    StepProcessor(Rebuild * aRebuild):rebuild(aRebuild)
    {
    }
    
    
    
    virtual void RunStep()
    {
    
    }
    void exitProcessing();
    
    
};


class EchoProcessor:public StepProcessor {
    
    
    void RunStep();
public:
    
    EchoProcessor(Rebuild * aRebuild):StepProcessor(aRebuild)
    {
    }

    
};


#endif /* StepProcessor_hpp */

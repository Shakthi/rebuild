//
//  StepProcessor.hpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 02/02/17.
//  Copyright © 2017 self. All rights reserved.
//

#ifndef StepProcessor_hpp
#define StepProcessor_hpp

#include "Rebuild.hpp"
#include <stdio.h>

class StepProcessor : public Rebuild::Serialised {
protected:
    class Rebuild* rebuild;
    class SentenceHistory *  history;


public:
    StepProcessor(Rebuild* aRebuild)
        : rebuild(aRebuild)
    {
    }


     SentenceHistory * GetHistory()const{
        return history;
    }
    virtual void RunStep() {}
    void exitProcessing();
    virtual ~StepProcessor() {}
};

class EchoProcessor : public StepProcessor {
    void RunStep();

public:
    EchoProcessor(Rebuild* aRebuild)
        : StepProcessor(aRebuild)
    {
    }
};

#endif /* StepProcessor_hpp */

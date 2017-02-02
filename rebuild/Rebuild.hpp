//
//  Rebuild.hpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 31/01/17.
//  Copyright © 2017 self. All rights reserved.
//

#ifndef Rebuild_hpp
#define Rebuild_hpp

#include <stack>
#include "StepProcessor.hpp"

class Rebuild {
    
    
    
    bool alive;
    enum  ExitStatus
    {
        exitStatusRIP=0,
        exitStatusLowMemory=3
        
    } exitStatus;
    
    
    
    
    
    std::stack<StepProcessor*> processorStack;
    
    
public:
    class LineNoiseWrapper * lineNoiseWrapper;

    
public:
    
    bool IsAlive() { return alive;}
    ExitStatus GetExitStatus() { return exitStatus;}
    
    Rebuild();
    ~Rebuild();
    void Save();
    void Load();
    
    
    void RunStep();
    void exitProcessing(StepProcessor*);
    
    
};


#endif /* Rebuild_hpp */



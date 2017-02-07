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

#include "json.hpp"

class Rebuild {
    
    
    
    bool alive;
    enum  ExitStatus
    {
        exitStatusRIP=0,
        exitStatusLowMemory=3
        
    } exitStatus;
    
    
    
    
    std::string GetSavePath();
    std::string LocalSavePath();
    
    std::stack<class StepProcessor*> processorStack;
    
    
public:
    class LineNoiseWrapper * lineNoiseWrapper;
    
    nlohmann::json lastStepProcessorData;

    
public:
    
    
    struct Serialised
    {
        virtual nlohmann::json ToJson() = 0;
        virtual void FromJson(nlohmann::json) = 0;
    
    };
    
    
    bool IsAlive() { return alive;}
    ExitStatus GetExitStatus() { return exitStatus;}
    
    Rebuild();
    ~Rebuild();
    void Save();
    void SaveIfLatest();
    void Load();
    
    
    void RunStep();
    void exitProcessing();
    void addNewProcessing(StepProcessor*);
    
    
    
    
    
};


#endif /* Rebuild_hpp */



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
#include <vector>

#include "nlohmann/json.hpp"
#include "VarTable.hpp"




class Rebuild {
   
    bool alive;
    enum ExitStatus {
        exitStatusRIP = 0,
        exitStatusLowMemory = 3

    } exitStatus;

    std::string GetSavePath();
    std::string LocalSavePath();
    

    std::stack<std::shared_ptr<class StepProcessor>> processorStack;
    std::vector<class SentenceHistory *> historyStack;
    std::vector<std::string> arglist;

    VarTable varTable;

public:
    class LineNoiseWrapper* lineNoiseWrapper;
    class SentenceHistory * history;
    static std::string prompt;
    nlohmann::json lastStepProcessorData;

public:
    struct Serialised {
        virtual nlohmann::json ToJson() = 0;
        virtual void FromJson(nlohmann::json) = 0;
    };

    const std::vector<class SentenceHistory*> & GetHistoryStack(){ return historyStack ;}

    bool IsAlive() { return alive; }

    ExitStatus GetExitStatus() { return exitStatus; }

    Rebuild(const std::vector<std::string> & argv);
    ~Rebuild();
    void Save();
    void SaveIfLatest();
    void Load();


    void RunStep();
    void exitProcessing();
    void addNewProcessing(std::shared_ptr<StepProcessor>);
    
    bool restart();
    
  
};

#endif /* Rebuild_hpp */

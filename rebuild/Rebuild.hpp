//
//  Rebuild.hpp
//  rebuild
//  Manager class the for the entair project. Note this is not a singleton class instead. Its create and destoryed from main funtion.
//  Created by Shakthi Prasad G S on 31/01/17.
//  Copyright © 2017 self. All rights reserved.
//

#ifndef Rebuild_hpp
#define Rebuild_hpp

#include <stack>
#include <vector>

#include "VarTable.hpp"
#include "nlohmann/json.hpp"

#include "SentenceHistory.hpp"
#include "linenoise/lineNoiseWrapper.hpp"

class StepProcessor;

class Rebuild {

public:
    enum ExitStatus {
        exitStatusRIP = 0,
        exitStatusLowMemory = 3
    };

    Rebuild(const std::vector<std::string>& argv);
    ~Rebuild();

    bool IsAlive() { return alive; }
    ExitStatus GetExitStatus() { return exitStatus; }

    void Load();
    void Save();
    void SaveIfLatest();  //Save the sission if its the latest running session
    bool Restart();       //Saves current session and hard restarts. Usefull during building the rebuild itself

    void RunStep();         //Core step of repl
    void Substep(const StepProcessor * stepProcessor); //Modal runstep execution

    void ExitProcessing();   //Exit the current stack of execution

    void AddNewProcessing(std::shared_ptr<StepProcessor>); //Enter new stack of execution

    const std::vector<SentenceHistory*>& GetHistoryStack() { return historyStack; }
    const std::string GetPrompt();

    SentenceHistory history;
    LineNoiseWrapper lineNoiseWrapper;  //Wrapper to cute linedit library https://github.com/antirez/linenoise

    void InitProcessor();

private:
    ExitStatus exitStatus;
    nlohmann::json lastStepProcessorData; //FIXME: Need to removed. Once full session restore is implemented
    std::string GetSavePath();            // Realved saved data path 
    std::string GetLocalSavePath();          // Saved data relative to project (for vcs)

    std::stack<std::shared_ptr<StepProcessor>> processorStack; //Stack of exection
    std::vector<SentenceHistory*> historyStack;
    std::vector<std::string> arglist;

    std::list<std::string> lastPromptList;
    std::string lastPromptString;



    VarTable varTable;
    bool alive;

    bool IsTopStepProcessor(const StepProcessor * stepProcessor)   ;
};

#endif /* Rebuild_hpp */
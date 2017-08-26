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

    void ExitProcessing();   //Exit the current stack of execution
    void AddNewProcessing(std::shared_ptr<StepProcessor>); //Enter new stack of execution

    const std::vector<SentenceHistory*>& GetHistoryStack() { return historyStack; }
    static const std::string& GetPrompt() { return prompt; } //FIXME:should be static?

    SentenceHistory history;
    LineNoiseWrapper lineNoiseWrapper;  //Wrapper to cute linedit library https://github.com/antirez/linenoise

    bool IsTopStepProcessor(const StepProcessor*);

private:
    ExitStatus exitStatus;
    static std::string prompt;
    nlohmann::json lastStepProcessorData; //FIXME: Need to removed. Once full session restore is implemented
    std::string GetSavePath();            // Realved saved data path 
    std::string GetLocalSavePath();          // Saved data relative to project (for vcs)

    std::stack<std::shared_ptr<StepProcessor>> processorStack; //Stack of exection
    std::vector<SentenceHistory*> historyStack;
    std::vector<std::string> arglist;


    VarTable varTable;
    bool alive;
};

#endif /* Rebuild_hpp */
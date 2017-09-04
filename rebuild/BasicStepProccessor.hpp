//
//  BasicStepProccessor.hpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 04/02/17.
//  Copyright © 2017 self. All rights reserved.
//

#ifndef BasicStepProccessor_hpp
#define BasicStepProccessor_hpp
#include "Rebuild.hpp"

#include "StepProcessor.hpp"
#include "VarTable.hpp"
#include "Sentence.h"

class BasicStepProcessor : public StepProcessor {

protected:
    VarTable  localVarTable;
    std::string prompt;
    



public:

    struct StepContext{
        bool handled;
        bool addtoHistory;
        bool macroSubstituted;

        StepContext(){
            handled =addtoHistory =true;
            macroSubstituted=false;
        }
    } ;

    


    void RunStep();
    virtual void ProcessStatement(StatementRef input,StepContext & aStepContext);
    virtual void ProcessCommand(CommandRef input,StepContext & aStepContext);
    virtual void ProcessSentence(SentenceRef result,StepContext & aStepContext);

    virtual SentenceRef ProcessInput(std::string input,LineNoiseWrapper::ExtraResults extraResults, StepContext & aStepContext);
    virtual  std::string ProcessCtrlKeyStroke(int ctrlchar);
    virtual void UpdateHistory(SentenceRef result,StepContext & stepContext);


    virtual void ProcessStep(std::string input,LineNoiseWrapper::ExtraResults & result,StepContext & aStepContext);

    virtual std::string ProcessByMacros(std::string input,LineNoiseWrapper::ExtraResults & result,StepContext & aStepContext);

    virtual void ExecuteStatement(StatementRef input);
    std::string GetPrompt();
    std::string SetPrompt(std::string prompt);


    BasicStepProcessor(Rebuild* aRebuild,VarTable * varTable)
        : StepProcessor(aRebuild)
    {
        history = &aRebuild->history;

        localVarTable.SetSuper(varTable);
    }

    nlohmann::json ToJson();
    void FromJson(nlohmann::json);
};
#endif /* BasicStepProccessor_hpp */

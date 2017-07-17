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




public:

    struct CmdResult{
        bool handled;
        bool addtoHistory;
    };



    void RunStep();
    virtual CmdResult Evaluate(StatementRef input);
    virtual CmdResult Process(std::shared_ptr<Command> input);
    virtual std::string ProcessCtrlKeyStroke(int ctrlchar);


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

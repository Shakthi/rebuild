//
//  ForStepProcessor.hpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 14/02/17.
//  Copyright © 2017 self. All rights reserved.
//

#ifndef ForStepProcessor_hpp
#define ForStepProcessor_hpp

#include "BasicStepProccessor.hpp"
#include "Value.h"
#include "StackedSentenceHistory.hpp"
#include "AST.hpp"
#include "VarTable.hpp"



class ForStepProcessor : public BasicStepProcessor {
    std::shared_ptr<ForStatment> thisForBlock;
    
    std::string remarks;
    StackedSentenceHistory stackedSentenceHistory;
    bool initConditionPassed;
    bool needToRewindHistory;
    
    //30
    //29
    //10
    //checkback
    //##
public:
    
    enum class InitType {normal,stepin,reload} initType;
    
    ForStepProcessor(Rebuild* aRebuild, std::shared_ptr<ForStatment> forStatement,VarTable * superTable,InitType ainitType = InitType::normal)
    : BasicStepProcessor(aRebuild,superTable),thisForBlock(forStatement),stackedSentenceHistory(aRebuild->GetHistoryStack()),
    needToRewindHistory(false),
    initType(ainitType)
    {
        history = &stackedSentenceHistory;




    }
    
    float  getForVar()
    {
        return localVarTable.GetVar(thisForBlock->forVar).getNumVal();
    }

    static bool IsListableStatement(SentenceRef s)
    {
        StatementRef  st = std::dynamic_pointer_cast<Statement>(s);
        if (!st) {
            return false;
        }

        if(std::dynamic_pointer_cast<NextStatement>(st)){
            return false;
        }
        if(std::dynamic_pointer_cast<ErrorStatement>(st)){
            return false;
        }

        if(std::dynamic_pointer_cast<UnProcessedStatment>(st)){
            return false;
        }

        return true;


    }

    
    nlohmann::json ToJson()
    {
        nlohmann::json j;
        return j;
    }
    
    void FromJson(nlohmann::json j) {}
    
    void RunStep();
    void Init();
    
    void ExecuteHistory();
    void ExecuteAStep();
    void ExecuteTheStatment(StatementRef  statment );
    bool CheckCondition();
    void ExecuteIncrement(std::shared_ptr<ForStatment>  forstatement=nullptr);
    void ArchiveStatements();
    void UnArchiveStatements();


    //should be static
    void ExecuteStatments(std::shared_ptr<ForStatment> forstatement);
    
    virtual CmdResult Process(std::shared_ptr<Command> input);

    void AddToHistory(SentenceRef entry);


    
};



#endif /* ForStepProcessor_hpp */

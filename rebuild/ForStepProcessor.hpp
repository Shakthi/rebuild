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
#include "SentenceHistory.hpp"
#include "AST.hpp"
#include "VarTable.hpp"



class ForStepProcessor : public BasicStepProcessor {
    ForStatment * thisForBlock;
    
    std::string remarks;
    PopingLineSentenceHistory popingLineHistory;
    std::list<Statement*> statementStash;
    bool initConditionPassed;
    
    
    
public:
    
    enum class InitType {normal,stepin,reload};
    
    ForStepProcessor(Rebuild* aRebuild, ForStatment * forStatement,VarTable * superTable,InitType initType = InitType::normal)
    : BasicStepProcessor(aRebuild,superTable),thisForBlock(forStatement),popingLineHistory(aRebuild->GetHistoryStack())
    {
        history = &popingLineHistory;

        if(initType!= InitType::reload){
            for (auto st :forStatement->statements ) {
                popingLineHistory.Add(st);
            }
        }



    }
    
    float  getForVar()
    {
        return localVarTable.GetVar(thisForBlock->forVar).getNumVal();
    }

    static bool IsListableStatement(Sentence * s)
    {
        Statement * st = dynamic_cast<Statement*>(s);
        if (!st) {
            return false;
        }

        if(dynamic_cast<NextStatement*>(st)){
            return false;
        }
        if(dynamic_cast<ErrorStatement*>(st)){
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
    void ExecuteTheStatment(Statement * statment );
    bool CheckCondition();
    void ExecuteIncrement(ForStatment  * forstatement=nullptr);
    void ArchiveStatements();
    void UnArchiveStatements();


    //should be static
    void ExecuteStatments(ForStatment  * forstatement);
    
    virtual CmdResult Process(class Command* input);
    

    
};



#endif /* ForStepProcessor_hpp */

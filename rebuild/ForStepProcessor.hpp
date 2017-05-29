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
    bool passThrough;
    
    
    
public:
    
    enum class InitType {normal,stepin,reload};
    
    ForStepProcessor(Rebuild* aRebuild, ForStatment * forStatement,VarTable * super,InitType initType = InitType::normal)
    : BasicStepProcessor(aRebuild,super),thisForBlock(forStatement)
    {
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

    //should be static
    void ExecuteStatments(ForStatment  * forstatement);
    
    virtual bool Process(class Command* input);
    

    
};



#endif /* ForStepProcessor_hpp */

//
//  IfStepProccessor.hpp
//  rebuild
//
//  Created by Shakthi Prasad G S on 12/03/17.
//  Copyright © 2017 self. All rights reserved.
//

#ifndef IfStepProccessor_hpp
#define IfStepProccessor_hpp


#include "BasicStepProccessor.hpp"
#include "Value.h"
#include "SentenceHistory.hpp"
#include "AST.hpp"



class IfStepProcessor : public BasicStepProcessor {
    
    std::string remarks;
    std::shared_ptr<IfStatment> ifStatment;//TODO: should be boolean expression
    
    bool passThroughe;
  
    PopingLineSentenceHistory popingLineHistory;
    

public:
    IfStepProcessor(Rebuild* aRebuild,std::shared_ptr<IfStatment> aifStatment,VarTable * superVarTable)
    : BasicStepProcessor(aRebuild,superVarTable),ifStatment(aifStatment),popingLineHistory(aRebuild->GetHistoryStack())
    {
        // getForVar()=forblock.forBegin->;
       // isInited=false;
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
    
    
    
};

#endif /* IfStepProccessor_hpp */
